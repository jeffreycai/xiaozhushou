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
}

