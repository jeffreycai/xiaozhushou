<?php

class TigtagUser {
  private $id;
  private $username;
  private $password;
  private $cookie_path;
  private $created;
  private $valid;
  
  public function __construct($username = null, $password = null, $cookie_path = null) {
    $this->username = $username;
    $this->password = $password;
    $this->cookie_path = $cookie_path;
  }
  
  public function setId($id) {
    $this->id = $id;
  }
  public function getId() {
    return $this->id;
  }
  
  public function setUsername($username) {
    $this->username = $username;
  }
  public function getUsername() {
    return $this->username;
  }
  
  public function setPassword($password) {
    $this->password = $password;
  }
  public function getPassword() {
    return $this->password;
  }
  
  public function setCookiePath($path) {
    $this->cookie_path = $path;
  }
  public function getCookiePath() {
    return $this->cookie_path;
  }
  
  public function setCreated($created) {
    $this->created = $created;
  }
  public function getCreated() {
    return $this->created;
  }
  
  public function setValid($valid) {
    $this->valid = $valid;
  }
  public function getValid() {
    return $this->valid;
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
      $user->initializeViaDbRecord($record);
      return $user;
    }
      return null;
  }
  
  public static function getUserById($id) {
    $result = db_select('xiaozhushou_tigtag_user', 'tu')
      ->fields('tu')
      ->condition('tu.id', $id)
      ->range(0,1)
      ->execute();
    if ($record = $result->fetch()) {
      $user = new TigtagUser();
      $user->initializeViaDbRecord($record);
      return $user;
    }
    return null;
  }
  
  public function initializeViaDbRecord($record) {
    $this->setId($record->id);
    $this->setUsername($record->username);
    $this->setPassword($record->password);
    $this->setCookiePath(DRUPAL_ROOT . $record->cookie_path);
    $this->setCreated($record->created);
    $this->setValid($record->valid);
  }
}