<?php
/**
 * Craw Rental list and store in db
 */
function sydneytoday_rental_list_craw() {  
  // load settings
  $util = Utility::getInstance();
  $url = $util->getSetting('sydneytoday->list_url');
  
  // read the list page
  $crawler = $util->getCrawler();
  $html = $crawler->read($url);
  
  // traverse dom and store items in db
  module_load_include('php', 'xiaozhushou_utilities', 'libraries/simple_html_dom');
  $dom = str_get_html($html);
  foreach ($dom->find('table') as $table) {
    if ($table->style == 'border-bottom: 1px dashed #CCC;') {
      echo $table;
    }
  }
}