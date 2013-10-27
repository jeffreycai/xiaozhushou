<?php
class Utility {
  public static function getInstance() {
    static $inst = null;
    if ($inst === null) {
      $inst = new Utility();
    }
    return $inst;
  }

  private function __construct() {

  }

  /**
   * Load all settings
   */
  public static function loadSettings($module) {
    $settings = Setting::getInstance();
    $settings->load($module);
  }
  
  /**
   * Get settings
   */
  public static function getSetting($path) {
    $settings = Setting::getInstance();
    return $settings->get($path);
  }
  
  /**
   * Load settings and constants
   */
  public static function bootstrap() {
    define("DS", DIRECTORY_SEPARATOR);
    self::loadSettings('xiaozhushao_utilities');
  }
}

