var tessel = require('tessel');
var gpio = tessel.port['GPIO'];
var accel = require('accel-mma84').use(tessel.port.A);
var _ = require('lodash');
var winston = require('winston');

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
  accel.on('data', function(readout) {
    winston.info(readout);
  });
});
