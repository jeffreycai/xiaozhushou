jQuery(function($){
  $('body').append('<div id="overlay"></div>');
  $('#overlay').append('<div class="inner"><i class="fa fa-home"></i> <span>悉尼</span>租房网 <br /><small>www.sydneyzufang.com</small><br /><small>房源搜索中 ...<i class="fa fa-refresh fa-spin"></i></small></div>');
});

function addOverlay() {
  if (!jQuery('body').hasClass('overlay')) {
    jQuery('body').addClass('overlay');
  }
}
function removeOverlay() {
  if (jQuery('body').hasClass('overlay')) {
    jQuery('body').removeClass('overlay');
  }
}

// This is to prevent FF "go back" issue
// http://stackoverflow.com/questions/2638292/after-travelling-back-in-firefox-history-javascript-wont-run
window.onunload = function(){}; 