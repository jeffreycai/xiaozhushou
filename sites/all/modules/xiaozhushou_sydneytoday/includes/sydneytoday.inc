<?php
/**
 * Craw Rental list and store in db
 */
function sydneytoday_rental_list_craw() {
  try {
    // load settings
    $util = Utility::getInstance();
    $urls = $util->getSetting('sydneytoday->list_url');

    foreach ($urls as $url) {
      // read the list page
      $crawler = $util->getCrawler();
      $html = $crawler->read($url);

      // traverse dom and store items in db
      module_load_include('php', 'xiaozhushou_utilities', 'libraries/simple_html_dom');
      $dom = str_get_html($html);
      foreach ($dom->find('table') as $table) {
        // locate a row
        if ($table->style == 'border-bottom: 1px dashed #CCC;') {
          $property_type = trim($table->find('td', 0)->plaintext);
          $title = trim($table->find('span', 0)->plaintext);

          $href = $table->find('a', 0)->href;
          $tokens = explode('=', $href);
          $rid = array_pop($tokens);

          // sanity check
          if (!preg_match('/^\d+$/', $rid)) {
            throw new Exception('Sydneytoday craw list error: rid is not integer');
          // import records
          } else {
            // check if exists already
            $existed = db_select('xiaozhushou_sydneytoday_rental_item', 'ri')
              ->condition('rid', $rid)
              ->countQuery()
              ->execute()->fetchAssoc();
            // if record already exists, quite
            if ($existed['expression'] == '1') {
              return;
            // otherwise, import to db
            } else {
              db_insert('xiaozhushou_sydneytoday_rental_item')
                ->fields(array(
                  'title' => $title,
                  'rid' => $rid,
                  'url' => $href,
                  'property_type' => $property_type,
                  'created_at' => time()
                ))
                ->execute();
            }
          }
        }
      }
      $dom->clear();
    }
  } catch (Exception $e) {
    $message = 'Sydneytoday sydneytoday_rental_list_craw() error:' . $e->getMessage();
    watchdog('xiaozhushou', $message, array(), WATCHDOG_ERROR);
  }
}

/**
 * Craw Rental list item and parse into db
 */
function sydneytoday_rental_item_parse($item) {
  try {
    // read the item page
    $html = Utility::getInstance()->getCrawler()->read($item['url']);
    module_load_include('php', 'xiaozhushou_utilities', 'libraries/simple_html_dom');
    $dom = str_get_html($html);

    $index = 0;
    $record = array();

    $trs = array();
    
    foreach ($dom->find('#becan_l tr') as $tr) {
      $trs[] = $tr;
    }
    foreach ($dom->find('#becan_r tr') as $tr) {
      $trs[] = $tr;
    }

    // grab property meta data
    foreach ($trs as $tr) {
      $label = $tr->find('td', 0);
      $val = $tr->find('td', 1);
      if ($label && $val) {
        $val = trim($val->plaintext);
        switch(str_replace(' ', '', trim($label->plaintext))) {
          case '房屋户型':
            $record['property_type'] = $val; break;
          case '出租方式': 
            $record['rental_type'] = $val; break;
          case '出租租金':
            $record['price'] = $val; break;
          case '房屋设备':
            $record['facilities'] = $val; break;
          case '附属设施':
            $record['near_by_facilities'] = $val; break;
          case '具体地址':
            $record['address'] = $val; break;
          case '联系人':
            $record['contact'] = $val; break;
          case '手机号码':
            $val = trim($tr->find('td', 1)->innertext);
            $record['mobile'] = $val; break;
          case '电话号码':
            $val = trim($tr->find('td', 1)->innertext);
            $record['phone'] = $val; break;
          case '电子邮箱':
            $val = trim($tr->find('td', 1)->innertext);
            $record['email'] = $val; break;
          case 'QQ号码':
            $record['qq'] = $val; break;
        }
      }

    }

    // grab published at
    $match = array();
    if (!$dom->find('#candytool', 0)) {
      throw new Exception('Can not find #candytool');
    }
    if (preg_match('/\d+\-\d+\-\d+/', $dom->find('#candytool', 0)->innertext, $match)) {
      $record['published_at'] = strtotime($match[0]);
    }

    // grab description
    if ($elem = $dom->find('#list_middle', 0)) {
      if ($elem = $elem->find('table', 0)) {
        if ($elem = $elem->children(2)) {
          if ($elem = $elem->find('div', 0)) {
            $comment = $elem->plaintext;
            // clean weird char as they cause error
            $record['comment'] = $comment;
          }
        }
      }
    }
//    $record['comment'] = $dom->find('#list_middle', 0)->find('table', 0)->children(2)->find('div', 0)->plaintext;
    // get images
    $record['images'] = array();
    foreach ($dom->find('#divmov_img a[rel="shadowbox"]') as $a) {
      $record['images'][] = $a->href;
    }
    $record['images'] = implode("\n", $record['images']);
    
    // set to crawed
    $record['crawed'] = 1;

    // dump to db
    $query = db_update('xiaozhushou_sydneytoday_rental_item')
      ->fields($record)
      ->condition('id', $item['id'])
      ->execute();
    
    $dom->clear();
    
  } catch (Exception $e) {
    $message = 'Sydneytoday sydneytoday_rental_item_parse() error for item["id"]='.$item['id'].' item["url"]='. $item['url'] . ' :' . $e->getMessage();
    watchdog('xiaozhushou', $message, array(), WATCHDOG_ERROR);
    db_update('xiaozhushou_sydneytoday_rental_item')
      ->fields(array('error' => $message))
      ->condition('id', $item['id'])
      ->execute();
  }
}

/**
 * Import Rental item as Drupal nodes
 */
function sydneytoday_rental_item_import($item) {
  try{
    global $user;
    $node = new stdClass();
    $node->title = $item['title'];
    $node->type = 'rental';
    node_object_prepare($node);
    $node->language = LANGUAGE_NONE;
    $node->uid = $user->uid;
    $node->status = 1; // published or not
    $node->promote = 0;
    $node->comment = 0;

    module_load_include('inc', 'xiaozhushou_utilities', 'includes/Utility.class');
    $util = Utility::getInstance();

    foreach ($item as $key => $val) {
      $field = null;
      // special treatment for "facilities" and "near_by_facilities" fields
      if (in_array($key, array('facilities', 'near_by_facilities'))) {
        $field = 'field_rental_' . $key . '_';
      // escape non-node fields
      } else if (in_array($key, array('published_at', 'created_at', 'crawed', 'imported'))) {
        continue;
      // special treatment for "images" field
      } else if ($key == 'images') {
        if (is_string($item['images']) && strlen($item['images'])) {
          $images = explode("\n", $item['images']);
          $imgs = array();
          $image_folder = $util->getInstance()->getSetting('sydneytoday->cron_image_store');
          if (!is_dir($image_folder) || !is_writable($image_folder)) {
            throw new Exception('Folder ' . $image_folder . ' does not exist or writable.');
          }

          foreach ($images as $url) {
            $image = file_get_contents($url);

            if ($file = file_save_data($image, $image_folder, FILE_EXISTS_RENAME)) {
              $imgs[] = (array)$file;
            }
          }
          $node->field_rental_images[LANGUAGE_NONE] = $imgs;
        }
        continue;
      } else {
        $field = 'field_rental_' . $key;
      }

      if (!is_null($field)) {
        if (!isset($node->$field)) {
          $value = array();
          $value[LANGUAGE_NONE] = array();
          $value[LANGUAGE_NONE][0] = array();
        } else {
          $value = $node->$field;
        }
        
        // format 'field_rental_comment'
        if ($field == 'field_rental_comment') {
          $val = format_field_rental_comment($val);
          $node->body[LANGUAGE_NONE][0]['value'] = $val;
          $node->body[LANGUAGE_NONE][0]['format'] = 'full_html';
        // for mobile and Email filed, if image url is given, fetch the remove image and put value as local image url
        } else if (in_array($field, $util->getSetting('sydneytoday->img_textfields'))) {
            $matches = array();
            if (preg_match('/(http:\/\/[^\'"]+)[\'"]/', $val, $matches)) {
                $num_url = $matches[1];
                $num = file_get_contents($num_url);
                $image_folder = $util->getInstance()->getSetting('sydneytoday->cron_image_store');
                if (($file = file_save_data($num, $image_folder, FILE_EXISTS_RENAME))) {
                    $value[LANGUAGE_NONE][0]['value'] = $file->uri;
                    $node->$field = $value;
                }
            }
        } else {
            $value[LANGUAGE_NONE][0]['value'] = $val;
            $node->$field = $value;
        }
      }
    }
//    echo "<pre>";
//    print_r($node);
//    die ("</pre>");
    node_save($node);
    
  } catch(Exception $e) {
    $message = "Sydneytoday sydneytoday_rental_item_import() error for item ID ".$item['id'].":" . $e->getMessage();
    watchdog('xiaozhushou', $message, array(), WATCHDOG_ERROR);
    db_update('xiaozhushou_sydneytoday_rental_item')
      ->fields(array('error' => $message))
      ->condition('id', $item['id'])
      ->execute();
  }
}

/**
 * format comment field before import as node
 * 
 * @param type $val
 */
function format_field_rental_comment($val) {
  $val = str_replace("\n", "<br />", $val);
  return $val;
//  $val = str_replace($, $subject);
}