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
      $row[] = l($user->cookie_path, 'http://' . $_SERVER['SERVER_NAME'] . $user->cookie_path);
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

/**
 * Check if a Tigtag user has a correct cookie that allows him to access authorised page
 * @param type $id
 * @return type 
 */
function tigtag_user_check($id) {
  module_load_include('inc', 'xiaozhushou_tigtag_user', 'includes/TigtagUser.class');
  $user = TigtagUser::getUserById($id);
  
  module_load_include('inc', 'xiaozhushou_utilities', 'includes/Crawler.class');
  $crawler = new Crawler();
  
  $crawler->setCookiePath($user->getCookiePath());
  $html = $crawler->read('http://bbs.tigtag.com/member.php?mod=logging&action=login');
//echo "<pre>"; print_r($html);die("</pre>");
  $valid = strpos($html, '<div id="messagetext" class="alert_right">') ? true : false;
  if ($valid) {
    drupal_set_message('User ' . $user->getUsername() . '\'s cookie is valid');
  } else {
    drupal_set_message('User ' . $user->getUsername() . '\'s cookie is invalid', 'warning');
  }
  
  db_update('xiaozhushou_tigtag_user') // Table name no longer needs {}
    ->fields(array(
      'valid' => $valid ? 1 : 0
    ))
    ->condition('id', $user->getId())
    ->execute();
  drupal_goto('admin/config/xiaozhushou/tigtag/user');
}

/**
 * Refresh a Tigtag user's cookie
 * @param type $form
 * @param type $form_state
 * @param type $id
 * @return string 
 */
function tigtag_user_refresh($form, &$form_state, $id) {
  // declare all vars used in form
  $seccode;
  $seccode_markup;
  $seccode_hash;
  $form_hash;
  $login_hash;
  $cookie_path;
  $username;
  $password;
  
  // if it is the first time this form is loaded, we fetch all vars for the form
  if (empty($form_state['input'])) {
    module_load_include('inc', 'xiaozhushou_tigtag_user', 'includes/TigtagUser.class');
    $user = TigtagUser::getUserById($id);

    if (is_null($user)) {
      throw new Exception('Error when trying to fetch user (id: '. $id . ') in tigtag_user_refresh()');
      return;
    }
    $username = $user->getUsername();
    $password = $user->getPassword();

    // clear cookie file
    $cookie_path = $user->getCookiePath();
    if ($file = fopen($cookie_path, 'w')) {
      fclose($file);
    }

    // login action
    module_load_include('inc', 'xiaozhushou_utilities', 'includes/Crawler.class');
    $crawler = new Crawler();
    $crawler->setCookiePath($cookie_path);
    $html = $crawler->read('http://bbs.tigtag.com/member.php?mod=logging&action=login');
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
    
    $dom->clear();
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
    $seccode_markup = '<img src="data:image/png;base64,' . base64_encode($seccode_content) . '" />';
  // if this form is submitted, we populate form vars with submit values
  } else {
    foreach ($form_state['input'] as $key => $val) {
      $$key = $val;
    }
  }
  
  $form = array();
  
  $form['captcha'] = array(
    '#type' => 'item',
    '#title' => 'Captcha'
  );
  $form['seccode_markup'] = array(
    '#type' => 'item',
    '#markup' => isset($seccode_markup) ? $seccode_markup : ''
  );
  $form['seccode'] = array(
    '#type' => 'textfield', 
    '#title' => t('Seccode'), 
    '#size' => 4, 
    '#maxlength' => 4, 
    '#required' => TRUE,
    '#default' => isset($seccode) ? $seccode : ''
  );
  $form['seccode_hash'] = array(
    '#type' => 'hidden',
    '#value' => isset($seccode_hash) ? $seccode_hash : ''
  );
  $form['form_hash'] = array(
    '#type' => 'hidden',
    '#value' => isset($form_hash) ? $form_hash : ''
  );
  $form['login_hash'] = array(
    '#type' => 'hidden',
    '#value' => isset($login_hash) ? $login_hash : ''
  );
  $form['cookie_path'] = array(
    '#type' => 'hidden',
    '#value' => isset($cookie_path) ? $cookie_path : ''
  );
  $form['username'] = array(
    '#type' => 'hidden',
    '#value' => isset($username) ? $username : ''
  );
  $form['password'] = array(
    '#type' => 'hidden',
    '#value' => isset($password) ? $password : ''
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

/**
 * Form submission action for tigtag_user_refresh()
 * @param type $form
 * @param array $form_state 
 */
function tigtag_user_refresh_submit($form, &$form_state) {
  $seccode = $form_state['input']['seccode'];
  $seccode_hash = $form_state['input']['seccode_hash'];
  $form_hash = $form_state['input']['form_hash'];
  $login_hash = $form_state['input']['login_hash'];
  $cookie_path = $form_state['input']['cookie_path'];
  $username = $form_state['input']['username'];
  $password = $form_state['input']['password'];
  
  $url = "http://bbs.tigtag.com/member.php?mod=logging&action=login&loginsubmit=yes&loginhash=$login_hash&inajax=1";
  $post_data = "formhash=$form_hash&referer=http%3A%2F%2Fbbs.tigtag.com%2F.%2F&loginfield=username&username=".$username."&password=".$password."&questionid=0&answer=&sechash=$seccode_hash&seccodeverify=$seccode&cookietime=2592000";

  $crawler = Utility::getInstance()->getCrawler();
  $crawler->setCookiePath($cookie_path);
  
  $html = $crawler->post($url, $post_data, $cookie_path);
  // die(iconv('GBK', 'UTF-8', $html));
  if (strstr($html, 'error')) {
    drupal_set_message('An error occurred, login failed.', 'error');
  } else {
    drupal_set_message('Cookie refreshed successfully!');
  }
  $form_state['redirect'] = 'admin/config/xiaozhushou/tigtag/user';
}

/**
 * Delete a Tigtag user from db
 * @param type $id
 * @return type 
 */
function tigtag_user_delete($id) {
  return 'tigtag_user_delete' . $id;
}


function tigtag_user_register() {
  return "tigtag_user_register";
}