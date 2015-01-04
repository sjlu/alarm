var tessel = require('tessel');
var gpio = tessel.port['GPIO'];
var _ = require('lodash');
var winston = require('winston');
var async = require('async');
var notes = require('./notes');

var led1 = tessel.led[0];
var led2 = tessel.led[1];
// var led3 = gpio.pin['G4'];
var pressureSensor = gpio.pin['G6'];
var speaker = gpio.pwm[0]; // 19
var vibrator = gpio.pin['G5']; // 18

// var accel = require('accel-mma84').use(tessel.port.A);
// var ambient = require('ambient-attx4').use(tessel.port.B);
// var climate = require('climate-si7020').use(tessel.port.C);
// var camera = require('camera-vc0706').use(tessel.port.D);

gpio.pwmFrequency(notes['C']);

var setSpeaker = function(state) {
  if (state) {
    speaker.pwmDutyCycle(0.99);
  } else {
    speaker.pwmDutyCycle(0);
  }
}

var setVibrator = function(state) {
  var set = 0;
  if (state) {
    set = 1;
  }

  vibrator.output(set);
}

var setAlarm = function(state) {
  setVibrator(state);
  setSpeaker(state);
}

var alarmTimeout;
var setTimer = function() {
  alarmTimeout = setTimeout(function() {
    setAlarm(true);
  }, 5000);
}
var clearTimer = function() {
  clearTimeout(alarmTimeout);
}

pressureSensor.on('rise', function() {
  led1.output(0);
  clearTimer()
  setAlarm(false);
});
pressureSensor.on('fall', function() {
  led1.output(1);
  setTimer();
});

// show ready
led2.output(1);
