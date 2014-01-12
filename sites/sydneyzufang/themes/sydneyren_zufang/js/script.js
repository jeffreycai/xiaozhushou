/**
 * @file
 * A JavaScript file for the theme.
 *
 * In order for this JavaScript to be loaded on pages, see the instructions in
 * the README.txt next to this file.
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - https://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function ($, Drupal, window, document, undefined) {


// To understand behaviors, see https://drupal.org/node/756722#behaviors
//Drupal.behaviors.my_custom_behavior = {
//  attach: function(context, settings) {
//
//    // Place your code here.
//    
//  }
//};

//  alter('hello world');


})(jQuery, Drupal, this, this.document);

jQuery(function($){
  // short cut links js action
  $('#block-block-1 a').click(function(){
    var suburb = $(this).html();
    $('#edit-search-api-views-fulltext').val(suburb);
    $('#views-exposed-form-rental-item-search-page').submit();
  });
  // search form select js action
  $('#views-exposed-form-rental-item-search-page select').change(function(){
    $(this).parents('form').submit();
  });
  // search form select "Any" link
  var parents = $('#views-exposed-form-rental-item-search-page select').parents('.views-exposed-widget');
  $('label', parents).append('<a href="#" style="font-size: 0.9em;">[ 任意 ]</a>');
  $('#views-exposed-form-rental-item-search-page label a').live("click", function(event){
    event.preventDefault();
    var parent = $(this).parents('.views-exposed-widget').first();
    if ($('select', parent).val() != 'All') {
      $('select', parent).val('All');
      $(this).parents('form').submit();
    }
  });
  
  // initialize flexslider 2
  $('#content article .flexslider').flexslider({
    animation: "slide"
  });
  
  // search breadcrumb
  if ($('#search-breadcrumb').length) {
    var suburb = $('#edit-field-rental-suburb option:selected').html();
    var property_type = $('#edit-field-rental-property-type option:selected').html();
    var rental_type = $('#edit-field-rental-rental-type option:selected').html();
    var keyword = $('#edit-search-api-views-fulltext').val();
    
    var wording = '当前搜索结果为: ';
    if (keyword != '') {
      wording += '关键词为"<span>'+keyword+'</span>", ';
    }
    wording += '位于"<span>'+(suburb == '- Any -' ? '任意地区' : suburb)+'</span>"， ';
    wording += '"<span>'+(rental_type == '- Any -' ? '任意出租形式' : rental_type) + '</span>"的';
    wording += '"<span>'+(property_type == '- Any -' ? '任意房屋类型' : property_type) +'</span>"';
    wording += '<br /><a href="#"><i class="fa fa-angle-double-up"></i> 我要修改搜索条件</a>';
    $('#search-breadcrumb').html(wording);
  }
  $('#search-breadcrumb a').click(function(){
    $('body,html').animate({
      scrollTop: $('#page-title').offset().top
    }, 800);
    return false;
  });
});