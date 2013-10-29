<?php
/**
 * tigtag_user_admin()
 */
function tigtag_user_admin() {
  $build['tigtag_user_list'] = drupal_get_form('tigtag_user_list');
}

/**
 * tigtag_user_list()
 * @return array
 */
function tigtag_user_list() {
  $header = array(
    'id' => array('data' => 'ID', 'field' => 'tu.id'),
    'username' => array('data' => 'Username', 'field' => 'tu.username'),
    'password' => array('data' => 'Password', 'field' => 'tu.password'),
    'created' => array('data' => 'Created at', 'field' => 'tu.created'),
    'valid' => array('data' => 'isValid?', 'field' => 'tu.valid'),
    'operations' => array('data' => 'operations'),
  );
  
  $query = db_select('xiaozhushou_tigtag_user', 'tu');
  
  $count_query = clone $query;
  $count_query->addExpression('COUNT(tu.id)');
  
  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('tu', array('id', 'username', 'password', 'created', 'valid'))
    ->limit(50)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();
  
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
  );
  $options = array();
  foreach (module_invoke_all('tigtag_user_operations') as $operation => $array) {
    $options[$operation] = $array['label'];
  }
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => 'unblock',
  );
  $options = array();
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );
  
  $destination = drupal_get_destination();
  
  
  
  return $form;
}

/**
 * tigtag_user_register()
 * @return string
 */
function tigtag_user_register() {
  return "<p>Registration</p>";
}
