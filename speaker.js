var tessel = require('tessel');
var gpio = tessel.port['GPIO'];
var _ = require('lodash');
var notes = require('./notes');

var speaker = gpio.pwm[0]; // 19
gpio.pwmFrequency(notes['C']);

var setSpeaker = function(state) {
  console.log(state);
  if (state) {
    speaker.pwmDutyCycle(0.99);
  } else {
    speaker.pwmDutyCycle(0);
  }
}

var state = false;
setInterval(function() {
  state = !state;
  setSpeaker(state);
}, 1000)

var led1 = tessel.led[0];
var led2 = tessel.led[1];

led2.output(1);