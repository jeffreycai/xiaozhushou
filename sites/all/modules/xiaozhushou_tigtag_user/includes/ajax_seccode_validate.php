<?php
// Drupal bootstrap
define('DRUPAL_ROOT', dirname(__FILE__) . '/../../../../..');
require_once(DRUPAL_ROOT . '/includes/bootstrap.inc');

chdir(DRUPAL_ROOT);
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

// get parameter from POST
$seccode_hash = $_POST['seccode_hash'];
$cookie_path = $_POST['cookie_path'];
$value = $_POST['value'];

// crawl
module_load_include('inc', 'xiaozhushou_utilities', 'includes/Crawler.class');
$crawler = new Crawler();
$crawler->setCookiePath($cookie_path);

//                         http://bbs.tigtag.com/misc.php?mod=seccode&action=check&inajax=1&&idhash=SQZGLDT0     &secverify=CRCB
$content = $crawler->read("http://bbs.tigtag.com/misc.php?mod=seccode&action=check&inajax=1&&idhash=$seccode_hash&secverify=$value", $cookie_path, false, true);

echo $content;