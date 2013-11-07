<?php
class Crawler {
  private $cookie_path;
  private $agent;
  
  public function read($url) {
    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1); // whether to print out or not when curl_exec();
    curl_setopt($ch, CURLOPT_HEADER, 0); // whether to include HEADER in output
//    curl_setopt($ch, CURLOPT_COOKIEJAR, $cookie_path); // where to put cookie after curl_close()
    curl_setopt($ch, CURLOPT_USERAGENT, $this->getUserAgent());

    $result = curl_exec($ch);
    curl_close($ch);
    return $result;
  }
  
  public function setCookiePath($path) {
    $this->cookie_path = $path;
  }
  
  public function getCookiePath() {
    if (!$this->cookie_path) {
      $this->cookie_path = __DIR__ . DS . 'cookies' . DS . 'temp_' . rand(1, 50) . '.txt';
    }
    return $this->cookie_path;
  }
  
  public function setUserAgent($agent) {
    if ($this->agent) {
      return $this->agent;
    } else {
      return 'User-Agent: Mozilla/5.0 (Windows NT 5.1; rv:24.0) Gecko/20100101 Firefox/24.0';
    }
  }
  
  public function getUserAgent() {
    return $this->agent;
  }
}