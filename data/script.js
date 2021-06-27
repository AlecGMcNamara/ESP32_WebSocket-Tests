var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

window.addEventListener('load', onload);

function  onload(event) {
  initWebSocket();
}

function initWebSocket() {
  console.log('Trying to open a WebSocket connection…');
  websocket = new WebSocket(gateway);
  websocket.onopen    = onOpen;
  websocket.onclose   = onClose;
  websocket.onmessage = onMessage;
}

function onOpen(event) {
  console.log('Connection opened');
}

function onClose(event) {
  console.log('Connection closed');
  setTimeout(initWebSocket, 2000);
} 

function onMessage(event) {
  var jsonReceived = JSON.parse(event.data);
    //set all objects on webpage after receiving message from server
    
    var myText = "";
    var myNumber  = 100;
    var myBool = false; 
    
    myText = jsonReceived["Message"];
    myNumber = jsonReceived["Number"];
    myBool = jsonReceived["Bool"]
    
    console.log(myText + " " + myNumber + " " + myBool);

    myText = "From Browser";
    myNumber = 69;
    myBool = true;


    //set up JSON message before sending mesage to server
    var jsonSend = {"Reply": myText,
                    "Number": myNumber,
                    "Bool": myBool};

    websocket.send(JSON.stringify(jsonSend));   
}
