<?php
define('USERNAME', 'jeanboy186');
define('PASSWORD', '0172122a');


$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
$_SERVER['SCRIPT_NAME'] = 'debug.php';

define('DRUPAL_ROOT','/var/webs/xiaozhushou.com.au');
require_once(DRUPAL_ROOT . '/includes/bootstrap.inc');
chdir(DRUPAL_ROOT);
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

module_load_include('inc', 'xiaozhushou_tigtag_user', 'TigtagUser.class');

// create cookie file
$cookie_path = tempnam('public://', 'cookie-');
if (!$cookie_path) {
  die("Failed to create cookie file: " . $cookie_path);
}

// login
$user = new TigtagUser(USERNAME, PASSWORD);
if ($user->login()) {
  
}


unlink($cookie_path);
echo 'done. ' . $cookie_path;
