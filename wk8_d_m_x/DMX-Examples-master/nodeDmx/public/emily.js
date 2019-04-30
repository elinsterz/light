let result = "hi topo!";


function setup(){
    createCanvas(200,200);
    console.log("hello");
}

function draw(){
    ellipse(mouseX, mouseY, 20);
}

function mouseReleased(){
    httpGet('/set/'+ mouseX + '/'+ mouseY, foo(result));
}

function foo(result){
    console.log("hi!");
}