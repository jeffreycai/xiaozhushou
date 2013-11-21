<?php

class TigtagUser {
  private $id;
  private $username;
  private $password;
  private $cookie_path;
  private $created;
  private $valid;
  
  public function __construct() {
    $this->username = $username;
    $this->password = $password;
    $this->cookie_path = $cookie_path;
  }
  
  public function setId($id) {
    $this->id = $id;
  }
  
  public function setUsername($username) {
    $this->username = $username;
  }
  
  public function setPassword($password) {
    $this->password = $password;
  }
  
  public function setCookiePath($path) {
    $this->cookie_path = $path;
  }
  
  public function setCreated($created) {
    $this->created = $created;
  }
  
  public function setValid($valid) {
    $this->valid = $valid;
  }
  
  /**
   * Get a valid user from db
   * @return type
   */
  public static function getValidUser() {
    $result = db_select('xiaozhushou_tigtag_user', 'tu')
      ->fields('tu')
      ->condition('tu.valid', 1)
      ->range(0,1)
      ->execute();
    if ($record = $result->fetch()) {
      $user = new TigtagUser();
      $user->setId($record->id);
      $user->setUsername($record->username);
      $user->setPassword($record->password);
      $user->setCookiePath($record->cookie_path);
      $user->setCreated($record->created);
      $user->setValid($record->valid);
      
      return $user;
    } else
      return null;
  }
  
}