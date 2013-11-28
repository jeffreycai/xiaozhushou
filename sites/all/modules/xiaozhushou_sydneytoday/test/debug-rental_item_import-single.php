<?php
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
$_SERVER['SCRIPT_NAME'] = '/debug.php';

define('DRUPAL_ROOT','/var/webs/sydneyren.com.au');
require_once(DRUPAL_ROOT . '/includes/bootstrap.inc');
$variables['url'] = 'http://zufang.websitesydney.net';
drupal_override_server_variables($variables);
chdir(DRUPAL_ROOT);
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);


echo "<pre>";
var_dump(file_get_stream_wrappers());
var_dump(drupal_realpath('public://'));
die("</pre>");

module_load_include('inc', 'xiaozhushou_sydneytoday', 'includes/sydneytoday');

$item = db_select('xiaozhushou_sydneytoday_rental_item','ri')
        ->fields('ri')
        ->range(0,1)
        ->execute()
        ->fetchAssoc();

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
      $image_folder = str_replace('public://', '', $util->getInstance()->getSetting('sydneytoday->cron_image_store'));
      $image_folder = DRUPAL_ROOT . DS . variable_get('file_public_path') . DS . $image_folder;
      if (!is_dir($image_folder) || !is_writable($image_folder)) {
        throw new Exception('Folder ' . $image_folder . ' does not exist or writable.');
      }

      foreach ($images as $url) {
        $image = file_get_contents($url);

        if ($file = file_save_data($image, 'public://cronimgs', FILE_EXISTS_REPLACE)) {
          $imgs[] = (array)$file;
        } else {
          unlink($path);
        }
      }
      $node->field_rental_images[LANGUAGE_NONE] = $imgs;
    }
    continue;
  } else {
    $field = 'field_rental_' . $key;
  }
  
  if (!is_null($field) && isset($node->$field)) {
    $value = $node->$field;
    $value[LANGUAGE_NONE][0]['value'] = $val;
    $node->$field = $value;
  }
}
//echo "<pre>";
//print_r($node);
//echo "</pre>";
node_save($node);
