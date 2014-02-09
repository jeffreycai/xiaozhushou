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
  $('ul.shortcut a').click(function(){
    var suburb = $(this).html();
    $('#edit-search-api-views-fulltext').val(suburb);
    $('#views-exposed-form-rental-item-search-search-result-list').submit();
  });
  // overlay actions
  removeOverlay();
  $('#views-exposed-form-rental-item-search-search-result-list').submit(function(){
    addOverlay();
  });

  // dummy search block action
  // link click
  $('#dummy-search ul li').click(function(){
    var parent = $(this).parent('ul');
    
    // highlight selected criteria
    if ($(this).hasClass('selected')) {
      $(this).removeClass('selected');
      // reset form field to "Any"
      $('#' + parent.attr('class')).val('All');
    } else {
      $('li', parent).removeClass('selected');
      $(this).addClass('selected');
    }
    
    // select the corresponding form field
    if ($(this).hasClass('selected')) {
      var val = $(this).attr('class').replace('selected', '').replace(' ', '');
      var target = $('#' + parent.attr('class'));
      target.val(val);
    }
  });
  // reverse link click
  $('#block-block-9 ul.edit-field-rental-suburb li.' + $('#edit-field-rental-suburb option:selected').val()).addClass('selected');
  $('#block-block-9 ul.edit-field-rental-property-type li.' + $('#edit-field-rental-property-type option:selected').val()).addClass('selected');
  $('#block-block-9 ul.edit-field-rental-rental-type li.' + $('#edit-field-rental-rental-type option:selected').val()).addClass('selected');
  // submission
  $('#block-block-9 .result input').click(function(){
    $('#views-exposed-form-rental-item-search-search-result-list').submit();
  });
  
  
  
  // initialize flexslider 2
  $('#content article .flexslider').flexslider({
    animation: "slide"
  });
  
  // search breadcrumb
  if ($('#search-breadcrumb').length && $('#content .view-empty').length == 0) {
    var suburb = $('#edit-field-rental-suburb option:selected').html();
    var property_type = $('#edit-field-rental-property-type option:selected').html();
    var rental_type = $('#edit-field-rental-rental-type option:selected').html();
    var keyword = $('#edit-search-api-views-fulltext').val();
    
    var wording = '当前搜索: ';
//    if (keyword != '') {
//      wording += '关键词为"<span>'+keyword+'</span>", ';
//    }
    wording += '位于"<span>'+(suburb == '- Any -' ? '任意地区' : suburb)+'</span>"， ';
    wording += '"<span>'+(rental_type == '- Any -' ? '任意出租形式' : rental_type) + '</span>"的';
    wording += '"<span>'+(property_type == '- Any -' ? '任意房屋类型' : property_type) +'</span>"';
    wording += '<br /><a href="#" class="search-again"><i class="fa fa-angle-double-up"></i> 我要修改搜索条件</a>';
    $('#search-breadcrumb').html(wording).show();
  }

  // search again
  $('a.search-again').click(function(){
    $('#block-block-9').animate({
      backgroundColor: '#FDECD1'
    }, 800, function(){
      $('#block-block-9').animate({
        backgroundColor: '#FFF'
      }, 800);
    });

    return false;
  });
  
  // image update form action
  $('#edit-field-rental-images input[type=file]').live('change', function(){
    var parent = $(this).parent();
    $('input[type=submit]', parent).fadeIn(800, function(){
      var bg_color = $(this).css('backgroundColor');
      $(this).animate({
        backgroundColor: '#FDECD1',
      }, 800, function(){
        $(this).animate({
          backgroundColor: bg_color
        }, 800);
      });
    });
  });
});