<?php
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
$_SERVER['SCRIPT_NAME'] = '/debug.php';

define('DRUPAL_ROOT','/var/webs/sydneyren.com.au');
require_once(DRUPAL_ROOT . '/includes/bootstrap.inc');
$variables['url'] = 'http://zufang.websitesydney.net';
drupal_override_server_variables($variables);
chdir(DRUPAL_ROOT);
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);


module_load_include('inc', 'xiaozhushou_sydneytoday', 'includes/sydneytoday');

sydneytoday_rental_list_craw();
