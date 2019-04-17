/*
Emily Lin
04/15/19

Notes:
Code edited from DMX examples
context: node.js

Credits: 
Shows how to use dmx library with an Enttec USB DMX Pro
For more on the library, see https://github.com/wiedi/node-dmx

based on the demos from the node-dmx repository
created 20 Mar 2017
by Tom Igoe
*/
var open = require('open');
var express = require('express'); // include the express library
var server = express();           // create a server using express

var DMX = require('dmx');     // include the dmx lib
var dmx = new DMX();          // create a new control instance
var sequence = DMX.Animation; // create a new animation sequence instance
var serialPort = '/dev/tty.usbserial-6A3L1L39';  // your serial port name

// create a new DMX universe on your serial port:::
var universe = dmx.addUniverse('mySystem',
  'enttec-usb-dmx-pro', serialPort);

var channel = 0;                        // channel number
var level = 0;                          // level
var fadeStep = 1;                       // increment to fade; for manual fading


// turn everything off:
for (channel=0; channel < 256; channel++) {
  var light = {[channel]: level};       // make an object
  universe.update(light);               // set channel to 0
}

//running a sequence using Animation:

var cue_1 = new sequence();
// animation.add(to, duration in ms, options for easing in ['linear'] format)
// animation.add (to, duration, options)
cue_1.add({70: 255, 71: 190, 72:175}, 500,['inBack']) //inBack one of the dmx easing options
  .delay(1000)
  .add({70: 255, 71: 140, 72:25},1000,['inExpo']);
//cue_1.run(universe, done);


var cue_2 = new sequence();
cue_2.add({70: 255}, 100,['inOutBounce'])  
  .add({71: 255},100,['inOutBounce'])
  .delay(1000);


var cue_3 = new sequence();
cue_3.add({70: 255, 71:144, 72:0}, 500,['inExpo'])  //easing option
  .delay(500)
  .add({70: 58, 71:150, 72:235},500,['inExpo']);


var cue_4 = new sequence();
cue_4.add({70: 10, 71:179, 72:193}, 2000,['inElastic'])  
  .add({71: 50},1000,['inElastic'])
  .delay(1000)
  .add({72: 50},1000,['inElastic']);

//array for all cue sequences
var cue_lib = [cue_1, cue_2, cue_3, cue_4];

function done() {
  console.log("done. Now I'll run the loop...");
  channel = 70;                          // reset channel and level
  level = 1;
  setInterval(fade, 20);                // run the fade every 20ms
}

// fading a channel manually:
function fade(){
  var light = {[channel]: level};       // put channel and level in JSON
  universe.update(light);               // update the light

  // change the level for next time:
  if (level === 255 || level === 0) {   // if 0 or 255
    fadeStep = -fadeStep;               // reverse the fade direction
    console.log('loop');
  }
  level += fadeStep;                    // increment/decrement the fade
}


function blackout(callback) {
  for (c=0; c < 256; c++) {
    var channel = {[c]: 0};       // make an object
    universe.update(channel);     // set channel to 0
  }
  callback;
}


//----------------------------------------------------
// from Tom's simpleBoardServer.js example
//----------------------------------------------------
function setChannel(request, response) {
  console.log('got a request. channel: ' + request.params.channel
  + ' level: ' + request.params.level);
  var channel = request.params.channel;
  var level = request.params.level;
  // set channel (most DMX systems are 1-indexed, but this library is 0-indexed):
  // universe.update({[channel-1]:level});
  if (channel <= 3){
    cue_lib[channel].run(universe,blackout());

  } else if (channel == 4) {
    blackout();
  }
  response.end('set ' + channel + ' to ' + level);
}

blackout();
server.use('/',express.static('public'));   // set a static file directory
server.listen(8080);
server.get('/set/:channel/:level', setChannel);
open('http://localhost:8080');              // open this URL in a browser


//----------------------------------------------------
// this section makes sure the script turns everything off
// before quitting:
function quit(error) {
  if (error) {
    console.log('Uncaught Exception: ');
    console.log(error.stack);
  }
  console.log('quitting');
  if (exitFunction) exitFunction();   // if there's an exit function, use it
  setTimeout(process.exit, 1000);     // avter 1 second, quit
}

// function blackout(callback) {
//   for (c=0; c < 256; c++) {
//     var channel = {[c]: 0};       // make an object
//     universe.update(channel);     // set channel to 0
//   }
//   callback;
// }

var exitFunction = blackout;

//Stop the script from quitting before you clean up:
process.stdin.resume();
process.on('SIGINT', quit);             // catch ctrl+c:
process.on('uncaughtException', quit);  //catch uncaught exceptions
process.on('beforeExit', quit);         // catch the beforeExit message



// var on = false;
// setInterval(function(){
//   if(on){
//     on = false;
//     universe.update({70: 100, 71: 0, 72: 0});
//     console.log("off");
//   }else{
//     on = true;
//     universe.update({70: 20, 71: 150, 72: 150});
//     console.log("on");
//   }
// }, 1000);


//Previous code for just turning on and off
// var on = false;
// setInterval(function(){
//   if(on){
//     on = false;
//     universe.updateAll(0);
//     console.log("off");
//   }else{
//     on = true;
//     universe.updateAll(250);
//     console.log("on");
//   }
// }, 1000);
