<?php
class Setting {
  private $vars;
    
  public static function getInstance() {
    static $inst = null;
    if ($inst === null) {
      $inst = new Setting();
    }
    return $inst;
  }

  private function __construct() {
    $this->vars = array();
  }
  
  /**
   * Load module settings to global singleton Setting instance
   * @param type $module
   */
  public function load($module) {
    // load spyc lib to parse yaml file
    module_load_include('php', 'xiaozhushou_utilities', 'libraries/spyc');
    $Spyc  = new Spyc;
    
    // load main settings file
    $main_settings_path = drupal_get_path('module', 'xiaozhushou_utilities') . DS . 'conf' . DS . 'settings.yml';
    $this->set($Spyc->load($main_settings_path));
    
    // load module settings, if it is not the main settings
    if ($module != 'xiaozhushou_utilities') {
      $module_settings_path = drupal_get_path('module', $module) . DS . 'conf' . DS . 'settings.yml';
      $this->set($Spyc->load($module_settings_path));
    }
  }
  
  /**
   * Set settings
   * @param type $arr
   */
  public function set($arr) {
    $this->vars = array_merge_recursive($this->vars, $arr);
  }
  
  /**
   * get settings
   * @param string $path, indicating the array keys of the setting that needs to be fetched
   * @return type
   */
  public function get($path) {
    $tokens = explode('|'); 
    $rtn = $this->vars;
    foreach ($tokens as $key) {
      $rtn = $rtn[$key];
    }
    return $rtn;
  }
}
