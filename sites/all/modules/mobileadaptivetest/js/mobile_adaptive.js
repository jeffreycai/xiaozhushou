(function ($) {
  
  $(document).ready(function() {
		// Set constants
		var blockPositionLeft = 15;
		// Set page path
		var path;
		if ($.browser.msie || $.browser.mozilla) {
			path = Drupal.settings.mobile_adaptive_path;
		}
		else {
			path = window.location.href;
		}
    var script_path = Drupal.settings.basePath + Drupal.settings.mobile_adaptive_script_path;
		// Check if script should be initialized
		if (!isInitAllowed()) {
			return;
		}
		// Init mobile adaptive script
    var mobileAdaptive = new MobileAdaptive(script_path, path);
    mobileAdaptive.init();
		// Set event handlers
    $('.mobile-adaptive-switch').click(function() {
			if (!$(this).hasClass('active')) {
				$(this).addClass('active');
				var device = $('.mobile-adaptive-block .configs.active .device.active').val();
				showDevice(device);
				$('.mobile-adaptive-block').animate({'left': blockPositionLeft});
			}
			else {
				$(this).removeClass('active');
				hideDevice();
				$('.mobile-adaptive-block').animate({'left': -210});
			}
    });

    $('.mobile-adaptive-block .mode-select div').click(function() {
			$('.mobile-adaptive-block .mode-select div').removeClass('active');
			$(this).addClass('active');
			$('.mobile-adaptive-block .configs.active').removeClass('active').hide();
			if ($(this).hasClass('phone-mode')) {
				$('.mobile-adaptive-block .configs.phone').addClass('active').show();
			}
			else if ($(this).hasClass('tablet-mode')) {
				$('.mobile-adaptive-block .configs.tablet').addClass('active').show();			
			}
			else if ($(this).hasClass('monitor-mode')) {
				$('.mobile-adaptive-block .configs.monitor').addClass('active').show();			
			}
			updateDevice();
    });
		
    $('.mobile-adaptive-block select.device').change(function() {
			updateDevice();
    });

    $('.mobile-adaptive-block select.os').change(function() {
			var os = $(this).val();
			$('.mobile-adaptive-block .configs.active .device.active').removeClass('active').hide();
			$('.mobile-adaptive-block .configs.active .device.' + os).addClass('active').show();
 			updateDevice();
   });
    
    $('.rotate').click(function() {
			if ($(this).hasClass('portrait')) {
				$(this).hide();
				$('.rotate.landscape').show();
			}
			else {
				$(this).hide();
				$('.rotate.portrait').show();
			}
      mobileAdaptive.rotate();
    });
    
		function updateDevice() {
			var device = $('.mobile-adaptive-block .configs.active .device.active').val();
			showDevice(device);
		}
		
    function showDevice(device) {
      mobileAdaptive.show(device);
			$('.rotate.landscape').hide();
			$('.rotate.portrait').show();
			$('.mobile-shadow').click(function() {
				$('.mobile-adaptive-switch').removeClass('active');
				hideDevice();
				$('.mobile-adaptive-block').animate({'left': -210});
			});
    }
		
		function hideDevice() {
			mobileAdaptive.hide();
		}    
		
		function isInitAllowed() {
			if (window.location.hash == '#no-mobile-adaptive') {
				return false;
			}
			return true;
		}
		
  });
  
  function MobileAdaptive(script_path, path) {
    this.blockId = 'mobile-adaptive';
    this.script_path = script_path;
    this.src = path + '#no-mobile-adaptive';
    this.isInit = false;
    this.animationTime = 250;
    this.horizontal = false;
    this.device;
    this.devices = ['iphone', 'ipad'];
    
    this.init = function() {
      var html = '<div class="mobile-adaptive-switch"></div>';
      html += '<div class="mobile-adaptive-block">';
			html += '<div class="mode-select">';
			html += '<div class="phone-mode active"></div>';
			html += '<div class="tablet-mode"></div>';
			html += '<div class="monitor-mode"></div>';
			html += '</div>';
			// Phone configs
			html += '<div class="configs phone active">';
			html += '<div class="os-select-wrapper">';
			html += '<select class="os">';
      html += '<option value="ios">iPhone</option>';
      html += '<option value="android">Android</option>';
			html += '</select>';
			html += '</div>';
			html += '<div class="device-select-wrapper">';
			/* Ios */
      html += '<select class="device ios active">';
      html += '<option value="iphone-3g">3g</option>';
      html += '<option value="iphone-4s">4s</option>';
      html += '<option value="iphone-5">5</option>';
      html += '</select>';
			/* Android */
      html += '<select class="device android">';
      html += '<option value="android-240x320">240x320</option>';
      html += '<option value="android-320x480">320x480</option>';
      html += '<option value="android-480x640">480x640</option>';
      html += '<option value="android-480x800">480x800</option>';
      html += '<option value="android-640x960">640x960</option>';
      html += '<option value="android-720x1280">720x1280</option>';
      html += '<option value="android-768x1024">768x1024</option>';
      html += '<option value="android-800x1280">800x1280</option>';
      html += '<option value="android-1200x1920">1200x1920</option>';
      html += '</select>';
			html += '</div>';
			html += '<div class="orientation">';
			html += '<div class="label">Orientation</div>';
      html += '<div class="landscape rotate" title="rotate">Landscape</div>';
      html += '<div class="portrait rotate" title="rotate">Portrait</div>';
			html += '</div>';
			html += '</div>';
			// Tablet configs
			html += '<div class="configs tablet">';
			html += '<div class="os-select-wrapper">';
			html += '<select class="os">';
      html += '<option value="ios">iPad</option>';
      html += '<option value="android">Android</option>';
			html += '</select>';
			html += '</div>';
			html += '<div class="device-select-wrapper">';
			/* Ios */
      html += '<select class="device ios active">';
      html += '<option value="ipad-1">iPad 1</option>';
      html += '<option value="ipad-2">iPad 2</option>';
      html += '<option value="ipad-3">iPad 3</option>';
      html += '<option value="ipad-mini">iPad mini</option>';
      html += '</select>';
			/* Android */
      html += '<select class="device android">';
      html += '<option value="android-tab-800x480">800x480</option>';
      html += '<option value="android-tab-800x600">800x600</option>';
      html += '<option value="android-tab-960x640">960x640</option>';
      html += '<option value="android-tab-1024x600">1024x600</option>';
      html += '<option value="android-tab-1024x768">1024x768</option>';
      html += '<option value="android-tab-1280x768">1280x768</option>';
      html += '<option value="android-tab-1280x800">1280x800</option>';
      html += '<option value="android-tab-1366x768">1366x768</option>';
      html += '<option value="android-tab-2560x1600">2560x1600</option>';
      html += '</select>';
			html += '</div>';
			html += '<div class="orientation">';
			html += '<div class="label">Orientation</div>';
      html += '<div class="landscape rotate" title="rotate">Landscape</div>';
      html += '<div class="portrait rotate" title="rotate">Portrait</div>';
			html += '</div>';
			html += '</div>';			
			// Monitor configs
			html += '<div class="configs monitor">';
			html += '<div class="device-select-wrapper">';
      html += '<select class="device active">';
      html += '<option value="monitor-macbook-air">Macbook Air</option>';
      html += '<option value="monitor-macbook-pro">Macbook Pro</option>';
      html += '<option value="monitor-800-600">Monitor 800x600</option>';
      html += '<option value="monitor-1600-900">Monitor 1600x900</option>';
      html += '</select>';
			html += '</div>';
			html += '</div>';

			html += '</div>';
      $('body').append(html);
    }
    
    this.getDeviceWrapperHtml = function(device) {
      var html = '<div id="' + this.blockId + '">';
      html += '<div class="mobile-shadow"></div>';
      html += '<div class="device-bg">';
      html += '<img class="image" src="' + this.script_path + '/images/models/' + device.name + '.png" />';
			html += '<div class="content"></div>';
      html += '</div>';
      html += '</div>';
      return html;
    };
    
    this.show = function(mode) {
      // Check if mobile adaptive emulator is active already.
      if (this.isInit) {
        this.destroy();
      }
      this.device = new DeviceInfo(mode);
      // Init wrapper.
      var wrapper = this.getDeviceWrapperHtml(this.device);
      $('body').append(wrapper);
      // Init device emulator.
      this.initWindow();
      // Set sizes
      if (!this.horizontal) {
        $('#' + this.blockId).addClass(this.device.name).addClass(this.device.cls).find('.content').width(this.device.screenWidth).height(this.device.screenHeight - this.device.menuHeight);
        $('#' + this.blockId).find('.device-bg').width(this.device.outerWidth).height(this.device.outerHeight);
      }
      else {
        $('#' + this.blockId).addClass(this.device.name + '-horizontal').find('.content').width(this.device.screenHeight).height(this.device.screenWidth);
        $('#' + this.blockId).find('.device-bg').width(this.device.outerHeight).height(this.device.outerWidth - this.device.menuHeight);
      }
      this.isInit = true;
    };
    
    this.initWindow = function() {
      if (!this.horizontal) {
        var iframeHtml = '<iframe src="' + this.src + '" width="' + this.device.screenWidth + '" height="' + (this.device.screenHeight - this.device.menuHeight) + '"></iframe>';
      }
      else {
        var iframeHtml = '<iframe src="' + this.src + '" width="' + this.device.screenHeight + '" height="' + (this.device.screenWidth - this.device.menuHeight) + '"></iframe>';
      }
      $('#' + this.blockId).find('.content').html(iframeHtml);
    }
    
    this.hide = function() {
			this.destroy();
    }
    
    this.rotate = function() {
      var mobileObject = this;
      if (this.horizontal) {
        $('#' + this.blockId).find('.device-bg').rotate({
          angle: -90,
          animateTo: 0,
          duration: this.animationTime,
          callback: function() {
            $('#' + mobileObject.blockId).find('iframe').fadeOut('fast', function() {
              $(this).width(mobileObject.device.screenWidth).height(mobileObject.device.screenHeight - mobileObject.device.menuHeight).css('-webkit-transform', '');
              $('#' + mobileObject.blockId).find('.device-bg').removeClass('horizontal').width(mobileObject.device.outerWidth).height(mobileObject.device.outerHeight);
							if (mobileObject.device.hasHorizontal) {
								$('#' + mobileObject.blockId).find('img').attr('src', mobileObject.script_path + '/images/models/' + mobileObject.device.name + '.png');
							}
              $(this).fadeIn('fast');
            });
          }
        });
        this.horizontal = false;
      }
      else {
        $('#' + this.blockId).find('.device-bg').rotate({
          angle: 0,
          animateTo: -90,
          duration: this.animationTime,
          callback: function() {
            $('#' + mobileObject.blockId).find('iframe').fadeOut('fast', function() {
              $(this).width(mobileObject.device.screenHeight).height(mobileObject.device.screenWidth - mobileObject.device.menuHeight).css('-webkit-transform', 'rotate(90deg)');
              $('#' + mobileObject.blockId).find('.device-bg').addClass('horizontal').width(mobileObject.device.outerHeight);
							if (mobileObject.device.hasHorizontal) {
								$('#' + mobileObject.blockId).find('img').attr('src', mobileObject.script_path + '/images/models/' + mobileObject.device.name + '-horizontal.png');
							}
              $(this).fadeIn('fast');
            });
          }
        });
        this.horizontal = true;
      }
    }
    
    this.destroy = function() {
      $('#' + this.blockId).remove();
      this.isInit = false;
      this.horizontal = false;
    }
    
  };
  
  function DeviceInfo(mode) {
    this.name = mode;
    switch (mode) {
			 /* Phones */
       case 'iphone-3g':
        this.screenWidth = 320;
        this.screenHeight = 480;
				this.menuHeight = 20;
        this.outerWidth = 373;
        this.outerHeight = 718;
				this.hasHorizontal = true;
       break;
       case 'iphone-4s':
        this.screenWidth = 640;
        this.screenHeight = 960;
				this.menuHeight = 40;
        this.outerWidth = 759;
        this.outerHeight = 1485;
				this.hasHorizontal = true;
       break;
       case 'iphone-5':
        this.screenWidth = 640;
        this.screenHeight = 1136;
				this.menuHeight = 42;
        this.outerWidth = 763;
        this.outerHeight = 1602;
				this.hasHorizontal = true;
       break;
       case 'android-240x320':
        this.screenWidth = 240;
        this.screenHeight = 320;
				this.menuHeight = 0;
        this.outerWidth = 284;
        this.outerHeight = 440;
				this.hasHorizontal = false;
       break;
       case 'android-320x480':
        this.screenWidth = 320;
        this.screenHeight = 480;
				this.menuHeight = 0;
        this.outerWidth = 365;
        this.outerHeight = 599;
				this.hasHorizontal = false;
       break;
       case 'android-480x640':
        this.screenWidth = 480;
        this.screenHeight = 640;
				this.menuHeight = 0;
        this.outerWidth = 545;
        this.outerHeight = 829;
				this.hasHorizontal = false;
       break;
       case 'android-480x800':
        this.screenWidth = 480;
        this.screenHeight = 800;
				this.menuHeight = 0;
        this.outerWidth = 544;
        this.outerHeight = 977;
				this.hasHorizontal = false;
       break;
       case 'android-640x960':
        this.screenWidth = 640;
        this.screenHeight = 960;
				this.menuHeight = 0;
        this.outerWidth = 727;
        this.outerHeight = 1193;
				this.hasHorizontal = false;
       break;
       case 'android-720x1280':
        this.screenWidth = 720;
        this.screenHeight = 1280;
				this.menuHeight = 0;
        this.outerWidth = 809;
        this.outerHeight = 1533;
				this.hasHorizontal = false;
       break;
       case 'android-768x1024':
        this.screenWidth = 768;
        this.screenHeight = 1024;
				this.menuHeight = 0;
        this.outerWidth = 856;
        this.outerHeight = 1318;
				this.hasHorizontal = false;
       break;
       case 'android-800x1280':
        this.screenWidth = 800;
        this.screenHeight = 1280;
				this.menuHeight = 0;
        this.outerWidth = 891;
        this.outerHeight = 1533;
				this.hasHorizontal = false;
       break;
       case 'android-1200x1920':
        this.screenWidth = 1200;
        this.screenHeight = 1920;
				this.menuHeight = 0;
        this.outerWidth = 1285;
        this.outerHeight = 2267;
				this.hasHorizontal = false;
       break;
			 /* Tablets */
       case 'ipad-1':
        this.screenWidth = 768;
        this.screenHeight = 1024;
				this.menuHeight = 24;
        this.outerWidth = 967;
        this.outerHeight = 1254;
				this.hasHorizontal = true;
       break;
       case 'ipad-2':
        this.screenWidth = 768;
        this.screenHeight = 1024;
				this.menuHeight = 24;
        this.outerWidth = 969;
        this.outerHeight = 1258;
				this.hasHorizontal = true;
			break;
			case 'ipad-3': 
        this.screenWidth = 1536;
        this.screenHeight = 2048;
				this.menuHeight = 40;
        this.outerWidth = 1923;
        this.outerHeight = 2509;
				this.hasHorizontal = true;
			break;
			case 'ipad-mini':
        this.screenWidth = 768;
        this.screenHeight = 1024;
				this.menuHeight = 28;
        this.outerWidth = 870;
        this.outerHeight = 1289;
				this.hasHorizontal = true;
			break;
			/* Android tab */
			case 'android-tab-800x480':
        this.screenWidth = 480;
        this.screenHeight = 800;
				this.menuHeight = 0;
        this.outerWidth = 617;
        this.outerHeight = 939;
				this.hasHorizontal = false;
				this.cls = 'android-tab';
			break;
			case 'android-tab-800x600':
        this.screenWidth = 600;
        this.screenHeight = 800;
				this.menuHeight = 0;
        this.outerWidth = 737;
        this.outerHeight = 939;
				this.hasHorizontal = false;
				this.cls = 'android-tab';
       break;
			case 'android-tab-960x640':
        this.screenWidth = 640;
        this.screenHeight = 960;
				this.menuHeight = 0;
        this.outerWidth = 817;
        this.outerHeight = 1139;
				this.hasHorizontal = false;
				this.cls = 'android-tab';
       break;
			case 'android-tab-1024x600':
        this.screenWidth = 600;
        this.screenHeight = 1024;
				this.menuHeight = 0;
        this.outerWidth = 777;
        this.outerHeight = 1203;
				this.hasHorizontal = false;
				this.cls = 'android-tab';
       break;
			case 'android-tab-1024x768':
        this.screenWidth = 768;
        this.screenHeight = 1024;
				this.menuHeight = 0;
        this.outerWidth = 945;
        this.outerHeight = 1203;
				this.hasHorizontal = false;
				this.cls = 'android-tab';
       break;
			case 'android-tab-1280x768':
        this.screenWidth = 768;
        this.screenHeight = 1280;
				this.menuHeight = 0;
        this.outerWidth = 958;
        this.outerHeight = 1499;
				this.hasHorizontal = false;
				this.cls = 'android-tab';
       break;
			case 'android-tab-1280x800':
        this.screenWidth = 800;
        this.screenHeight = 1280;
				this.menuHeight = 0;
        this.outerWidth = 1017;
        this.outerHeight = 1499;
				this.hasHorizontal = false;
				this.cls = 'android-tab';
       break;
			case 'android-tab-1366x768':
        this.screenWidth = 768;
        this.screenHeight = 1366;
				this.menuHeight = 0;
        this.outerWidth = 985;
        this.outerHeight = 1584;
				this.hasHorizontal = false;
				this.cls = 'android-tab';
       break;
			case 'android-tab-2560x1600':
        this.screenWidth = 1600;
        this.screenHeight = 2560;
				this.menuHeight = 0;
        this.outerWidth = 1918;
        this.outerHeight = 2878;
				this.hasHorizontal = false;
				this.cls = 'android-tab';
       break;
			 /* Monitors */
       case 'monitor-macbook-air':
        this.screenWidth = 1445;
        this.screenHeight = 900;
				this.menuHeight = 0;
        this.outerWidth = 1628;
        this.outerHeight = 1135;
				this.hasHorizontal = false;
       break;
       case 'monitor-macbook-pro':
        this.screenWidth = 2556;
        this.screenHeight = 1600;
				this.menuHeight = 0;
        this.outerWidth = 2819;
        this.outerHeight = 1965;
				this.hasHorizontal = false;
       break;
       case 'monitor-800-600':
        this.screenWidth = 800;
        this.screenHeight = 600;
				this.menuHeight = 0;
        this.outerWidth = 866;
        this.outerHeight = 830;
				this.hasHorizontal = false;
       break;
       case 'monitor-1600-900':
        this.screenWidth = 1600;
        this.screenHeight = 900;
				this.menuHeight = 0;
        this.outerWidth = 1740;
        this.outerHeight = 1378;
				this.hasHorizontal = false;
       break;
    }
  }
  
})(jQuery);