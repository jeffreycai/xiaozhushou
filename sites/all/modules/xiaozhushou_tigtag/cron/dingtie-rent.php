<?php
$fid = 48;
$tid = 3160720;

date_default_timezone_set('Asia/Shanghai');
$time = time();


$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
$_SERVER['SCRIPT_NAME'] = 'debug.php';

define('DRUPAL_ROOT','/var/webs/xiaozhushou.com.au');
require_once(DRUPAL_ROOT . '/includes/bootstrap.inc');

chdir(DRUPAL_ROOT);
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

// load ding tie messages
module_load_include('inc', 'xiaozhushou_utilities', 'includes/Utility.class');
$messages = Utility::getInstance()->getSetting('dingtie_messages');


// get a valid user
module_load_include('inc', 'xiaozhushou_tigtag_user', 'includes/TigtagUser.class');
$user = new TigtagUser();
$user = $user->getValidUser();
$cookie_path = $user->getCookiePath();

if (is_null($user)) {
  echo 'No valid user available';
  exit;
}

// craw login form and get form hash
module_load_include('inc', 'xiaozhushou_utilities', 'includes/Crawler.class');
$crawler = new Crawler();
$crawler->setCookiePath($cookie_path);
$headers = array(
      'Referer: http://bbs.tigtag.com/forum.php?mod=viewthread&tid=3160720&pid=31999233',
);
$url = "http://bbs.tigtag.com/forum.php?mod=post&action=reply&fid=$fid&tid=$tid";

$html = $crawler->read($url);
$matches = array();
preg_match('/id="formhash" value="([^"]+)"/', $html, $matches);
$formhash = isset($matches[1]) ? $matches[1] : null;

if (is_null($formhash)) {
  echo 'Error when getting form hash.';
}

// Post the comment
$url_post = "http://bbs.tigtag.com/forum.php?mod=post&action=reply&fid=$fid&tid=$tid&extra=&replysubmit=yes&infloat=yes&handlekey=fastpost&inajax=1";
$data = "message=" . urlencode($messages[array_rand($messages)]) . "&posttime=$time&formhash=$formhash&usesig=1&subject=++";
$headers = array(
  'Referer: ' . $url,
);//die($data);
$html = $crawler->post($url_post, $data, $cookie_path, $headers);

//die(iconv('GBK', 'UTF-8', $html));
if (strpos($html, 'succeedhandle_fastpost')) {
  echo "Success!\n";
  echo iconv('GBK', 'UTF-8', $html);
} else {
  echo (iconv('GBK', 'UTF-8', $html));
  echo "\n";
}