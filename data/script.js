var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
var testNumber = 0;

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
    var myBool = false; 
    
    myText = jsonReceived["Message"];
    testNumber = jsonReceived["Number"];
    myBool = jsonReceived["Bool"]
    
    document.getElementById("testValue").innerHTML = testNumber;

    console.log(myText + " " + testNumber + " " + myBool);

    myText = "From Browser";
    myBool = true;


    //set up JSON message before sending mesage to server
    testNumber++;
    var jsonSend = {"Reply": myText,
                    "Number": testNumber,
                    "Bool": myBool};

    websocket.send(JSON.stringify(jsonSend));   
}
