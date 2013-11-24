<?php
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
$_SERVER['SCRIPT_NAME'] = 'debug.php';

define('DRUPAL_ROOT','/var/webs/xiaozhushou.com.au');
require_once(DRUPAL_ROOT . '/includes/bootstrap.inc');
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

chdir(DRUPAL_ROOT);

//die(shell_exec('whoami'));


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
      foreach ($images as $url) {
        $image = file_get_contents($url);
        $name = basename($url);
        if ($file = file_save_data($image, $util->getSetting('cache_folder') . '/' . $name, FILE_EXISTS_RENAME)) {
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
    $value = $node->$field;
    $value[LANGUAGE_NONE][0]['value'] = $val;
    $node->$field = $value;
  }
}
//echo "<pre>";
//print_r($node);
//echo "</pre>";
node_save($node);
