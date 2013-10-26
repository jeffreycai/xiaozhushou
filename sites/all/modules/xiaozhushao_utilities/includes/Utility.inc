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

  public function 
}
