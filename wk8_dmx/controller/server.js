var http = require('http');
var module1 = require('./module1'); //same dir use ./
var module2 = require('./module2'); //same dir use ./

function onRequest(request, response){
    response.writeHead(200,{'Content-Type':'text/plain'});
    response.write('Hello');
    module2.myFunction();
    response.end();
}

http.createServer(onRequest).listen(8000);  //creates server and listen to port 8000




