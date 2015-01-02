var tessel = require('tessel');
var gpio = tessel.port['GPIO'];
var accel = require('accel-mma84').use(tessel.port.A);
var _ = require('lodash');
var winston = require('winston');
var ambient = require('ambient-attx4').use(tessel.port.B);
var async = require('async');

var led1 = tessel.led[0].output(1);
var led2 = tessel.led[1].output(0);

setInterval(function() {
  led1.toggle();
  led2.toggle();
}, 100);

gpio.pwmFrequency(1);
var vibrateMotor = gpio.pwm[0];
vibrateMotor.pwmDutyCycle(0.5);

accel.on('ready', function() {
  accel.setOutputRate(1.56);
  accel.on('data', function(readout) {
    winston.info(readout);
  });
});

ambient.on('ready', function() {
  setInterval(function() {
    async.parallel({
      light: function(cb) {
        ambient.getLightLevel(cb);
      },
      sound: function(cb) {
        ambient.getSoundLevel(cb);
      }
    }, function(err, data) {
      if (err) winston.error(err);  
      winston.info(data);
    });
  }, 1000);
});
