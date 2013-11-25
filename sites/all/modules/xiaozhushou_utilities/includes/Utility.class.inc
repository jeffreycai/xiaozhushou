<?php
require_once __DIR__ . DIRECTORY_SEPARATOR . 'Setting.class.inc';

class Utility {
  public static function getInstance() {
    static $inst = null;
    if ($inst === null) {
      $inst = new Utility();
      $inst->bootstrap();
    }
    return $inst;
  }

  private function __construct() {

  }

  /**
   * Load all settings
   */
  public function loadSettings($module) {
    $settings = Setting::getInstance();
    $settings->load($module);
  }
  
  /**
   * Get settings
   */
  public function getSetting($path = "") {
    $settings = Setting::getInstance();
    return $settings->get($path);
  }
  
  /**
   * Load settings and constants
   */
  private function bootstrap() {
    $this->loadSettings('xiaozhushou_utilities');
  }
  
  /**
   * get a page crawler
   * @return \Crawler
   */
  public function getCrawler() {
    module_load_include('inc', 'xiaozhushou_utilities', 'includes/Crawler.class');
    
    $crawler = new Crawler();
    return $crawler;
  }
  
  /**
   * function to generate a username
   * @param type $length
   * @return string 
   */
  function generateUsername() {
    $characters = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $randomString = '';
    for ($i = 0; $i < 5; $i++) {
        $randomString .= $characters[rand(0, strlen($characters) - 1)];
    }
    
    $characters = '0123456789';
    $rs = '';
    for ($i = 0; $i < 3; $i++) {
        $rs .= $characters[rand(0, strlen($characters) - 1)];
    }
    
    return $randomString.$rs;
  }
  
  /**
   * Generate a gmail alias address for $base
   *
   * @param type $base
   * @param type $postfix
   * @return type 
   */
  function generateEmail($base) {
    if (!preg_match('/.+@.+/i', $base)) {
      throw new Exception('Email base name needs to be valid');
    }

    $tokens = explode('@', $base);
    $chars = str_split($tokens[0]);
    $length = sizeof($chars);
    
    $rtn = '';
    $occurence = array();
    $insert_poses = array();
    
    for ($i = 0; $i < rand(1, $length); $i++) {
      $insert_pos = rand(0, $length - 1);
      if (in_array($insert_pos, $occurence)) {
        continue;
      } else {
        $occurence[] = $insert_pos;
        $insert_poses[] = $insert_pos;
      }
    }
    
    for ($i = 0; $i < $length; $i++) {
      $rtn .= $chars[$i];
      if (in_array($i, $insert_poses)) {
        $rtn .= '.';
      }
    }
    
    return $rtn . '@' . $tokens[1];

  }
}

