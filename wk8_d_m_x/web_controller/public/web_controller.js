let canvas_width = 800;
let canvas_height = 200;

let angle = 0;

//buttons
let btn_width = canvas_width / 4;
let btn_height = canvas_height;

let btn_x = [0, canvas_width * 0.25, canvas_width * 0.5, canvas_width * 0.75];
let btn_y = [0, 0, 0, 0];

let col_arr_0 = [
  [255, 255, 255],
  [255, 255, 255],
  [255, 255, 255],
  [255, 255, 255]
];

let col_arr = [
  [255, 255, 10],
  [214, 0, 229],
  [221, 255, 0],
  [35, 9, 234]
];


function setup() {
  createCanvas(canvas_width, canvas_height);
}

function draw() {

  background(225);
  //rectMode(CENTER);

  for (let i = 0; i < 4; i++) {
    stroke(0);
    strokeWeight(2);
    fill(col_arr_0[i]);
    rect(btn_x[i], btn_y[i], btn_width, btn_height);
  }
}

function mousePressed() {

  for (let m = 0; m < 4; m++) {

  checkLocation(btn_x[m], btn_y[m], m);
    
    // send messages to the DMX light at this point to 
    // change the light animation
    // [0,0,0] would turn everything off;
    
  }
}


//////Functions used in the script

// function checks the location of mouse within the bounds of the box / button

function checkLocation(xloc, yloc, j) {

  //check to see if mouse is over that quadrant
  if ((mouseX > xloc) && (mouseX < xloc + btn_width) &&
    (mouseY > yloc) && (mouseY < yloc + btn_height)) {
    console.log(col_arr_0[j]);

    let comp_test = arr_comp(col_arr_0[j], col_arr[j]);      
    console.log(comp_test);
      
      //if mouse is over quadrant and it is white, then go through color array
      //compares 2 arrays, if box color turns to color send an animation command
      //if box turns white from color then send a command to stop
      if (comp_test == false) {
        col_arr_0[j] = col_arr[j];
        httpGet('/set/'+ 71 + '/'+ 255, foo("Turn On!"));
      } else {
        col_arr_0[j] = [255, 255, 255];
        httpGet('/set/'+ 71 + '/'+ 0, foo("Turn Off!"));
      }
    }
  }


function foo(info_msg){
  console.log(info_msg)
}



// Function comparing two arrays
// Retruns false if not equal, returns true if equal

function arr_comp(arr1,arr2){
 
  var elem_test = true; 
  
      for (let k = 0; k < 3; k++) {
      if(arr1[k] === arr2[k]){
         elem_test = true; 
      } else {
        elem_test = false;
        return false;
      } 
  
}
  return true;
}

