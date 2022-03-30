var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

window.addEventListener('load', onload);

var Function1Pressed = false;
var Function2Pressed = false;
var Function3Pressed = false;
var Function4Pressed = false;

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
    
    setLed(jsonReceived["ip1"], document.getElementById("ledinput1"),"/leds/green.png" ,"/leds/green_off.png" );
    setLed(jsonReceived["ip2"], document.getElementById("ledinput2"),"/leds/green.png" ,"/leds/green_off.png" );
    setLed(jsonReceived["ip3"], document.getElementById("ledinput3"),"/leds/green.png" ,"/leds/green_off.png" );
    setLed(jsonReceived["ip4"], document.getElementById("ledinput4"),"/leds/green.png" ,"/leds/green_off.png" );

    setLed(jsonReceived["op1"], document.getElementById("ledoutput1"),"/leds/red.png" ,"/leds/red_off.png" );
    setLed(jsonReceived["op2"], document.getElementById("ledoutput2"),"/leds/red.png" ,"/leds/red_off.png" );
    setLed(jsonReceived["op3"], document.getElementById("ledoutput3"),"/leds/red.png" ,"/leds/red_off.png" );
    setLed(jsonReceived["op4"], document.getElementById("ledoutput4"),"/leds/red.png" ,"/leds/red_off.png" );
    console.log("Received Message");
}
function sendMessage() 
{
  //set up JSON message before sending mesage to server
    var jsonSend = {"fb1": Function1Pressed,
                    "fb2": Function2Pressed,
                    "fb3": Function3Pressed,
                    "fb4": Function4Pressed};

    websocket.send(JSON.stringify(jsonSend));  
    Function1Pressed = false;
    Function2Pressed = false;
    Function3Pressed = false;
    Function4Pressed = false;
}
function fb1Click ()
{
  Function1Pressed = true;
  sendMessage();
}

function fb2Click ()
{
  Function2Pressed = true;
  sendMessage();
}
function fb3Click ()
{
  Function3Pressed = true;
  sendMessage();
}
function fb4Click ()
{
  Function4Pressed = true;
  sendMessage();
}


function setLed(ledStatus, Led, ledOn, ledOff)
{
    if (ledStatus)    {
      Led.src=ledOn;  }
    else              {
      Led.src=ledOff; }
}
