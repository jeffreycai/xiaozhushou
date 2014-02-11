<?php
// Drupal bootstrap
define('DRUPAL_ROOT', dirname(__FILE__) . '/../../../../..');
require_once(DRUPAL_ROOT . '/includes/bootstrap.inc');

chdir(DRUPAL_ROOT);
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

// get parameter from POST
$seccode_hash = $_POST['seccode_hash'];
$cookie_path = $_POST['cookie_path'];

// crawl
module_load_include('inc', 'xiaozhushou_utilities', 'includes/Crawler.class');
$crawler = new Crawler();
$crawler->setCookiePath($cookie_path);

//                         http://bbs.tigtag.com/misc.php?mod=seccode&action=update&idhash=SJ1IAuS0     &inajax=1&ajaxtarget=seccode_SJ1IAuS0
$content = $crawler->read("http://bbs.tigtag.com/misc.php?mod=seccode&action=update&idhash=$seccode_hash&inajax=1&ajaxtarget=seccode_$seccode_hash", $cookie_path, false, true);
$matches = array();
preg_match('/src="(([^"]+)update=(\d+)[^"]+)"/', $content, $matches);
//             http://bbs.tigtag.com/    misc.php?mod=seccode&update=38082&idhash=SJ1IAuS0
$update_url = "http://bbs.tigtag.com/" . $matches[1];
$update_id = $matches[3];

//  // fetch the seccode
$header = array(
  'Referer: http://bbs.tigtag.com/member.php?mod=logging&action=login',
);
$seccode_content = $crawler->read($update_url, $cookie_path, $header, true);
$seccode_markup = base64_encode($seccode_content);

echo $seccode_markup;