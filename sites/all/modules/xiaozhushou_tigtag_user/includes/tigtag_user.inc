<?php

/**
 * tigtag_user_admin()
 */
function tigtag_user_overview() {
  $records = db_select('xiaozhushou_tigtag_user', 'tu')
    ->fields('tu')
    ->execute();
  
  if (!$records) {
    echo "There is no user at the moment.";
  } else {
    $header = array('id', 'username', 'password', 'cookie path', 'created', 'valid', array('data' => 'operations', 'colspan' => '3'));
    $rows = array();
    while ($user = $records->fetch()) {
      $row = array();
      $row[] = $user->id;
      $row[] = $user->username;
      $row[] = $user->password;
      $row[] = $user->cookie_path;
      $row[] = date('Y-m-d H:i:s', $user->created);
      $row[] = $user->valid;
      $row[] = l('Check validation', 'admin/config/xiaozhushou/tigtag/user/' . $user->id . '/check');
      $row[] = l('Refresh cookie', 'admin/config/xiaozhushou/tigtag/user/' . $user->id . '/refresh');
      $row[] = l('Delete', 'admin/config/xiaozhushou/tigtag/user/' . $user->id . '/delete');
      $rows[] = $row;
    }
    return theme('table', array('header' => $header, 'rows' => $rows));
  }
}

function tigtag_user_check($id) {
  return 'tigtag_user_check: ' . $id;
}

function tigtag_user_refresh($form, &$form_state, $id) {
  module_load_include('inc', 'xiaozhushou_tigtag_user', 'includes/TigtagUser.class');
  $user = TigtagUser::getUserById($id);
  
  if (is_null($user)) {
    throw new Exception('Error when trying to fetch user (id: '. $id . ') in tigtag_user_refresh()');
    return;
  }
  
  // delete cookie file
  $cookie_path = $user->getCookiePath();
  if (is_file($cookie_path)) {
    unlink($cookie_path);
  }
  
  // login action
  module_load_include('inc', 'xiaozhushou_utilities', 'includes/Crawler.class');
  $crawler = new Crawler();var_dump($crawler->getCookiePath());die();
  $crawler->setCookiePath($cookie_path);
  $html = $crawler->read('http://bbs.tigtag.com/member.php?mod=logging&action=login', $cookie_path);
  module_load_include('php', 'xiaozhushou_utilities', 'libraries/simple_html_dom');
  $dom = str_get_html($html);
  // get form hashes
  $form_hash = $dom->find('input[name="formhash"]', 0)->value;
  $seccode_hash = $dom->find('input[name="seccodeverify"]', 0)->id;
  $tokens = explode('_', $seccode_hash);
  $seccode_hash = array_pop($tokens);
  $login_hash = $dom->find('input[name="cookietime"]', 0)->id;
  $tokens = explode('_', $login_hash);
  $login_hash = array_pop($tokens);
  // get form seccode
  // refresh seccode
  $content = $crawler->read("http://bbs.tigtag.com/misc.php?mod=seccode&action=update&idhash=$seccode_hash&inajax=1&ajaxtarget=seccode_$seccode_hash", $cookie_path);
  $matches = array();
  preg_match('/src="(([^"]+)update=(\d+)[^"]+)"/', $content, $matches);
  $update_url = "http://bbs.tigtag.com/" . $matches[1];
  $update_id = $matches[3];
//  // fetch the seccode
  $header = array(
    'Referer: http://bbs.tigtag.com/member.php?mod=logging&action=login',
  );
  $seccode_content = $crawler->read($update_url, $cookie_path, $header);
  
  $form = array();
  
  $form['captcha'] = array(
    '#type' => 'item',
    '#title' => 'Captcha'
  );
  $form['seccode'] = array(
    '#type' => 'item',
    '#markup' => '<img src="' . $seccode_content . '" />'
  );
  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array(
      '#type' => 'submit', 
      '#value' => t('Login')
  );
  $form['#submit'][] = 'tigtag_user_refresh_submit';
  return $form;
}

function tigtag_user_refresh_submit($form, &$form_state) {
  die('done!');
}

function tigtag_user_delete($id) {
  return 'tigtag_user_delete' . $id;
}

function tigtag_user_register() {
  return "tigtag_user_register";
}