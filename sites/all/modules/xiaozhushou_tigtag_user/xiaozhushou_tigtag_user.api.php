<?php

function hook_tigtag_user_operations() {
  $operations = array(
    'delete' => array(
      'label' => t('Delete the selected users'),
      'callback' => 'tigtag_user_operations_delete',
    ),
  );
  return $operations;
}