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

function tigtag_user_refresh($id) {
  return 'tigtag_user_refresh' . $id;
}

function tigtag_user_delete($id) {
  return 'tigtag_user_delete' . $id;
}

function tigtag_user_register() {
  return "tigtag_user_register";
}