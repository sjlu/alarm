var tessel = require('tessel');
var gpio = tessel.port['GPIO'];
var _ = require('lodash');

var led1 = tessel.led[0];
var led2 = tessel.led[1];

var sensor = gpio.pin['G6'];
var analog = gpio.pin['A1'];

setInterval(function() {
  console.log(analog.read());
}, 100);

sensor.on('rise', function() {
  led1.output(0)
});
sensor.on('fall', function() {
  led1.output(1);
});

led2.output(1);

