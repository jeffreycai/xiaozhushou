<?php
/**
 * Craw Rental list and store in db
 */
function sydneytoday_rental_list_craw() {
  try {
    // load settings
    $util = Utility::getInstance();
    $url = $util->getSetting('sydneytoday->list_url');

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
            break;
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
  } catch (Exception $e) {
    watchdog('xiaozhushou', 'Sydneytoday sydneytoday_rental_list_craw() error:' . $e->getMessage(), array(), WATCHDOG_ERROR);
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
    foreach ($dom->find('#becan_l', 0)->find('tr') as $tr) {
      $trs[] = $tr;
    }
    foreach ($dom->find('#becan_r', 0)->find('tr') as $tr) {
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
    if (preg_match('/\d+\-\d+\-\d+/', $dom->find('#candytool', 0)->innertext, $match)) {
      $record['published_at'] = strtotime($match[0]);
    }

    // grab description
    $record['comment'] = $dom->find('#list_middle', 0)->find('table', 0)->children(2)->find('div', 0)->plaintext;

    $record['cron_queued'] = 1;

    // dump to db
    $query = db_update('xiaozhushou_sydneytoday_rental_item')
      ->fields($record)
      ->condition('id', $item['id'])
      ->execute();
  } catch (Exception $e) {
    watchdog('xiaozhushou', 'Sydneytoday sydneytoday_rental_item_parse() error for item %url: ' . $e->getMessage(), array('%url' => $item['url']), WATCHDOG_ERROR);
  }
}