<?php

class TigtagUser {
  /**
   * Get a valid user from db
   * @return type
   */
  public function getValidUser() {
    return db_select('xiaozhushou_tigtag_user', 'tu')
      ->fields('tu')
      ->condition('tu.valid', 1)
      ->limit(1)
      ->execute()
      ->fetchAssoc();
  }
}