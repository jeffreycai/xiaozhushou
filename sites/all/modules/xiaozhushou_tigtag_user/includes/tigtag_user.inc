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
    $header = array('id', 'username', 'password', 'cookie path', 'created', 'valid', array('data' => 'operations', 'colspan' => '4'));
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
      $row[] = l('Delete', 'admin/config/xiaozhushou/tigtag/user/' . $user->id . '/delete', array(
          'attributes' => array('class' => 'delete-user')
      ));
      $row[] = l('Promote', 'admin/config/xiaozhushou/tigtag/user/' . $user->id . '/promote', array(
          'attributes' => array('class' => 'promote-user')
      ));
      $rows[] = $row;
    }
    
    drupal_add_js('jQuery(function ($) { 
      $("a.delete-user").click(function(){
        if (confirm("Do you really want to delete this user?") == true) {
          return true;
        }
        return false;
      });
    });', 'inline');
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
 * Promote a user to the first location
 * 
 * @param type $id
 */
function tigtag_user_promote($id) {
  $first_id = db_select('xiaozhushou_tigtag_user', 'tu')
    ->fields('tu')
    ->orderBy('tu.id', 'ASC')
    ->range(0,1)
    ->execute()
    ->fetch()
    ->id;
  
  // move the first to a temp id (99)
  db_update('xiaozhushou_tigtag_user')
    ->fields(array(
      'id' => 99
    ))
    ->condition('id', $first_id)
    ->execute();
  
  // update the current to first id
  db_update('xiaozhushou_tigtag_user')
    ->fields(array(
      'id' => $first_id
    ))
    ->condition('id', $id)
    ->execute();
  
  // update the first record to current id
  db_update('xiaozhushou_tigtag_user')
    ->fields(array(
      'id' => $id
    ))
    ->condition('id', 99)
    ->execute();
  
  drupal_set_message('User has been successfully promoted!');
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
    $html = $crawler->read('http://bbs.tigtag.com/member.php?mod=logging&action=login', $cookie_path = null, $headers = false, $tor = true);
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
    $content = $crawler->read("http://bbs.tigtag.com/misc.php?mod=seccode&action=update&idhash=$seccode_hash&inajax=1&ajaxtarget=seccode_$seccode_hash", $cookie_path, false, true);
    $matches = array();
    preg_match('/src="(([^"]+)update=(\d+)[^"]+)"/', $content, $matches);
    $update_url = "http://bbs.tigtag.com/" . $matches[1];
    $update_id = $matches[3];
  //  // fetch the seccode
    $header = array(
      'Referer: http://bbs.tigtag.com/member.php?mod=logging&action=login',
    );
    $seccode_content = $crawler->read($update_url, $cookie_path, $header, true);
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
  
  $html = $crawler->post($url, $post_data, $cookie_path, $headers = false, $tor = false);
  // die(iconv('GBK', 'UTF-8', $html));
  if (strstr($html, 'error')) {
    drupal_set_message('An error occurred, login failed:<br />' . htmlspecialchars(iconv('GBK', 'UTF-8', $html)), 'error');
  } else {
    drupal_set_message('Cookie refreshed successfully!<br />' . htmlspecialchars(iconv('GBK', 'UTF-8', $html)));
  }
  $form_state['redirect'] = 'admin/config/xiaozhushou/tigtag/user';
}

/**
 * Delete a Tigtag user from db
 * @param type $id
 * @return type 
 */
function tigtag_user_delete($id) {
  $path = current_path();
  
  db_delete('xiaozhushou_tigtag_user')
    ->condition('id', $id)
    ->execute();
    
  drupal_set_message('Tigtag user ' . $id . ' is successfully deleted!');
  drupal_goto('admin/config/xiaozhushou/tigtag/user');
}

/**
 * Register a tigtag user
 * 
 * @param type $form
 * @param type $form_state
 * @return string
 */
function tigtag_user_register($form, &$form_state) {
  // declare all vars used in form
  $seccode;
  $seccode_markup;
  $seccode_hash;
  $formhash;
//  $register_hash;
  $cookie_path;
  $username;
  $username_id;
  $password;
  $password_id;
  $password_confirm;
  $password_confirm_id;
  $email;
  $email_id;
  $status;
  $status_id;
  $cellphone;
  $cellphone_id;
  $referer;
  $regsubmit;
  
  // if it is the first time this form is loaded, we fetch all vars for the form
  if (empty($form_state['input'])) {
    module_load_include('inc', 'xiaozhushou_tigtag_user', 'includes/TigtagUser.class');
    $user = new TigtagUser();
    $cookie_folder = DRUPAL_ROOT . DS . drupal_get_path('module', 'xiaozhushou_tigtag_user') . DS . 'cache';
    $cookie_path = tempnam($cookie_folder, 'user-');

    // login action
    module_load_include('inc', 'xiaozhushou_utilities', 'includes/Utility.class');
    $crawler = Utility::getInstance()->getCrawler();
    $crawler->setCookiePath($cookie_path);
    $html = $crawler->read('http://bbs.tigtag.com/member.php?mod=registerzui');
    module_load_include('php', 'xiaozhushou_utilities', 'libraries/simple_html_dom');
    $dom = str_get_html($html);
    // get form field info
    $regsubmit = $dom->find('#registerform input[name="regsubmit"]', 0)->value;
    $formhash = $dom->find('#registerform input[name="formhash"]', 0)->value;
    $referer = 'http://bbs.tigtag.com/';
    
    $seccode_hash = $dom->find('#registerform input[name="seccodeverify"]', 0)->id;
    $tokens = explode('_', $seccode_hash);
    $seccode_hash = array_pop($tokens);
    
    $username_id = $dom->find('#reginfo_a .rfm', 0)->find('input', 0)->id;
    $password_id = $dom->find('#reginfo_a .rfm', 1)->find('input', 0)->id;
    $password_confirm_id = $dom->find('#reginfo_a .rfm', 2)->find('input', 0)->id;
    $email_id = $dom->find('#reginfo_a .rfm', 3)->find('input', 0)->id;
    $status_id = $dom->find('#reginfo_a .rfm', 4)->find('input', 0)->name;
    $status = $dom->find('#reginfo_a .rfm', 4)->find('input', 0)->value;
    $cellphone_id = $dom->find('#reginfo_a .rfm', 5)->find('input', 0)->name;
    
//echo "<pre>";
//print_r(array(
//   '$regsubmit' => $regsubmit,
//    '$formhash' => $formhash,
//    '$referer' => $referer,
//    '$seccode_hash' => $seccode_hash,
//    '$username_id' => $username_id,
//    '$password_id' => $password_id,
//    '$password_confirm_id' => $password_confirm_id,
//    '$email_id' => $email_id,
//    '$status_id' => $status_id,
//    '$status' => $status,
//    '$cellphone_id' => $cellphone_id
//));
//die("</pre>");
    
    
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
  
  $default_username = Utility::getInstance()->generateUsername();
  $form['username'] = array(
    '#type' => 'textfield',
    '#title' => 'Username',
    '#required' => TRUE,
    '#value' => isset($username) ? $username : $default_username,
  );
  $form['username_id'] = array(
    '#type' => 'hidden',
    '#value' => isset($username_id) ? $username_id : ''
  );
  
  $form['password'] = array(
    '#type' => 'textfield',
    '#title' => 'Password',
    '#required' => TRUE,
    '#value' => '0172122a'
  );
  $form['password_id'] = array(
    '#type' => 'hidden',
    '#value' => isset($password_id) ? $password_id : ''
  );
  
  $form['password_confirm_id'] = array(
    '#type' => 'hidden',
    '#value' => isset($password_confirm_id) ? $password_confirm_id : ''
  );
  
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email',
    '#required' => TRUE,
    '#value' => isset($email) ? $email : Utility::getInstance()->generateEmail('findloveinsydney@gmail.com'),
  );
  $form['email_id'] = array(
    '#type' => 'hidden',
    '#value' => isset($email_id) ? $email_id : ''
  );
  
  $form['cellphone'] = array(
    '#type' => 'textfield',
    '#title' => 'Cellphone',
    '#required' => FALSE,
    '#value' => isset($cellphone) ? $cellphone : '',
  );
  $form['cellphone_id'] = array(
    '#type' => 'hidden',
    '#value' => isset($cellphone_id) ? $cellphone_id : ''
  );
  
  $form['status'] = array(
    '#type' => 'hidden',
    '#value' => isset($status) ? $status : ''
  );
  $form['status_id'] = array(
    '#type' => 'hidden',
    '#value' => isset($status_id) ? $status_id : ''
  );
  
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
    '#title' => 'Seccode',
    '#size' => 4, 
    '#maxlength' => 4, 
    '#required' => TRUE,
  );
  $form['seccode_hash'] = array(
    '#type' => 'hidden',
    '#value' => isset($seccode_hash) ? $seccode_hash : ''
  );
  $form['formhash'] = array(
    '#type' => 'hidden',
    '#value' => isset($formhash) ? $formhash : ''
  );
  $form['cookie_path'] = array(
    '#type' => 'hidden',
    '#value' => isset($cookie_path) ? $cookie_path : ''
  );
  $form['regsubmit'] = array(
    '#type' => 'hidden',
    '#value' => isset($regsubmit) ? $regsubmit : ''
  );
  $form['referer'] = array(
    '#type' => 'hidden',
    '#value' => isset($referer) ? $referer : ''
  );

  
  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Register')
  );
  $form['#submit'][] = 'tigtag_user_register_submit';
  return $form;
}

/**
 * Form submission action for tigtag_user_register()
 * 
 * @param type $form
 * @param array $form_state
 */
function tigtag_user_register_submit($form, &$form_state) {
  // get data from form
  $seccode = $form_state['input']['seccode'];
  $seccode_hash = $form_state['input']['seccode_hash'];
  $formhash = $form_state['input']['formhash'];
  $cookie_path = $form_state['input']['cookie_path'];
  $username = $form_state['input']['username'];
  $username_id = $form_state['input']['username_id'];
  $password = $form_state['input']['password'];
  $password_id = $form_state['input']['password_id'];
  $password_confirm = $form_state['input']['password'];
  $password_confirm_id = $form_state['input']['password_confirm_id'];
  $email = $form_state['input']['email'];
  $email_id = $form_state['input']['email_id'];
  $status = $form_state['input']['status'];
  $status_id = $form_state['input']['status_id'];
  $cellphone = $form_state['input']['cellphone'];
  $cellphone_id = $form_state['input']['cellphone_id'];
  $referer = $form_state['input']['referer'];
  $regsubmit = $form_state['input']['regsubmit'];
  
//echo "<pre>";
//print_r(array(
//  'seccode' => $seccode,
//  'seccode_markup' => $seccode_markup,
//  'seccode_hash' => $seccode_hash,
//  'formhash' => $formhash,
////  $register_hash;
//  'cookie_path' => $cookie_path,
//  'username' => $username,
//  'username_id' => $username_id,
//  'password' => $password,
//  'password_id' => $password_id,
//  'password_confirm' => $password_confirm,
//  'password_confirm_id' => $password_confirm_id,
//  'email' => $email,
//  'email_id' => $email_id,
//  'status' => $status,
//  'status_id' => $status_id,
//  'cellphone' => $cellphone,
//  'cellphone_id' => $cellphone_id,
//  'referer' => $referer,
//  'regsubmit' => $regsubmit,
//));
//die("</pre>");

  module_load_include('inc', 'xiaozhushou_utilities', 'includes/Utility.class');
  $crawler = Utility::getInstance()->getCrawler();
  
  $post_url = "http://bbs.tigtag.com/member.php?mod=registerzui&inajax=1";
  $header = array(
    'Referer: http://bbs.tigtag.com/member.php?mod=registerzui',
  );
  $post_data = array(
    'regsubmit' => $regsubmit,
    'formhash' => $formhash,
    'referer' => $referer,
    'activationauth' => '',
    $username_id => $username,
    $password_id => $password,
    $password_confirm_id => $password_confirm,
    $email_id => $email,
    $status_id => iconv('UTF-8', 'GBK', $status),
    $cellphone_id => $cellphone,
    'sechash' => $seccode_hash,
    'seccodeverify' => $seccode,
  );
  
  $html = $crawler->post($post_url, $post_data, $cookie_path, $header, true);
  
  // die(iconv('GBK', 'UTF-8', $html));
  if (strstr($html, 'error')) {
    drupal_set_message('An error occurred, login failed:<br />' . htmlspecialchars(iconv('GBK', 'UTF-8', $html)), 'error');
    if (is_file($cookie_path)) { unlink($cookie_path); }
  } else {
    drupal_set_message('Cookie refreshed successfully!<br />' . htmlspecialchars(iconv('GBK', 'UTF-8', $html)));
    
    module_load_include('inc', 'xiaozhushou_tigtag_user', 'includes/TigtagUser.class');
    $user = new TigtagUser();
    $user->setCookiePath($cookie_path);
    $user->setCreated(time());
    $user->setPassword($password);
    $user->setUsername($username);
    $user->setValid(1);
    $user->insert();
  }
  $form_state['redirect'] = 'admin/config/xiaozhushou/tigtag/user';
}