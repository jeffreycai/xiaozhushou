<?php

class TigtagUser {
  private $username;
  private $password;
  private $cookie_path;
  
  public function __construct($username = null, $password = null, $cookie_path = null) {
    $this->username = $username;
    $this->password = $password;
    $this->cookie_path = $cookie_path;
  }
  
  public function setUsername($username) {
    $this->username = $username;
  }
  
  public function setPassword($password) {
    $this->password = $password;
  }
  
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
  
  /**
   * Login to forum
   */
  public function login($username = null, $password = null, $cookie_path = null) {
    // paramaters validation
    if (is_null($username)) {
      $username = $this->username;
    }
    if (is_null($password)) {
      $password = $this->password;
    }
    if (is_null($cookie_path)) {
      $cookie_path = $this->cookie_path;
    }
    
    if (is_null($username) || is_null($password) || is_null($cookie_path)) {
      throw new Exception('Tigtag user login needs valid Username, Password and Cookie path');
      return false;
    }
    
    // start login
    module_load_include('inc', 'xiaozhushou_utilities', 'includes/Crawler.class');
    $crawler = new Crawler();
    $crawler->setCookiePath($cookie_path);
    $crawler->setUserAgent();
    
    
  }
}