var tessel = require('tessel');
var gpio = tessel.port['GPIO'];
var _ = require('lodash');
var winston = require('winston');
var async = require('async');

var led1 = tessel.led[0];
var led2 = tessel.led[1];
var led3 = gpio.pin['G4'];
var pressureSensor = gpio.pin['G6'];

// var accel = require('accel-mma84').use(tessel.port.A);
// var ambient = require('ambient-attx4').use(tessel.port.B);
// var climate = require('climate-si7020').use(tessel.port.C);
// var camera = require('camera-vc0706').use(tessel.port.D);

pressureSensor.on('rise', function() {
  led3.output(0);
});
pressureSensor.on('fall', function() {
  led3.output(1);
});
