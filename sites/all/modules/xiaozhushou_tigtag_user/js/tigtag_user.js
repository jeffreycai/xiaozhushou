jQuery(function($){
  $('#edit-seccode-markup').append('&nbsp;&nbsp;&nbsp;&nbsp;<span class="loading"><a href="#">Refresh</a></span>');
  $('#edit-seccode').after('&nbsp;&nbsp;&nbsp;&nbsp;<span class="validate"><a href="#">Validate</a></span>');

  // seccode refresh
  var seccode_hash = $('input[name="seccode_hash"]').val();
  var cookie_path = $('input[name="cookie_path"]').val();
  
  $('span.loading a').live('click', function(event){
    event.preventDefault();
    $('span.loading').html('Loading ...');
    $.ajax({
      type: "POST",
      url: '/sites/all/modules/xiaozhushou_tigtag_user/includes/ajax_seccode_refresh.php',
      data: 'seccode_hash='+seccode_hash+'&cookie_path='+cookie_path,
      success: function(data){
        $('#edit-seccode-markup img').attr('src', 'data:image/png;base64,'+data);
        $('span.loading').html('<a href="#">Refresh</a>');
      }
    });
  });
  
  // seccode validate
  $('span.validate a').live('click', function(event){
    event.preventDefault();
    $('span.validate').html('Validating ...');
    
    var value = $('#edit-seccode').val();
    // ajax to validate
    var seccode_hash = $('input[name="seccode_hash"]').val();
    $.ajax({
      type: "POST",
      // http://bbs.tigtag.com/misc.php?mod=seccode&action=check&inajax=1&&idhash=SQZGLDT0&secverify=CRCB
      url: '/sites/all/modules/xiaozhushou_tigtag_user/includes/ajax_seccode_validate.php',
      data: 'seccode_hash='+seccode_hash+'&cookie_path='+cookie_path+'&value='+value,
      success: function(data){
        $('span.validate').html('<a href="#">Validate</a>');
        alert(data);
      }
    });
  });
});