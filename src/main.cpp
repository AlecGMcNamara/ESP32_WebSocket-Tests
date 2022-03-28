#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ArduinoJSON.h>

const char* ssid = "SKYPEMHG";
const char* password = "8NHetSWQAJ75";

bool output1 = false;
bool output2 = true;
bool output3 = false;
bool output4 = true;

bool input1 = false;
bool input2 = true;
bool input3 = false;
bool input4 = true;

bool function1clicked = false;
bool function2clicked = false;
bool function3clicked = false;
bool function4clicked = false;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

String hostname = "ESP32Test";
void initWiFi() 
{

  WiFi.mode(WIFI_STA);
  WiFi.setHostname(hostname.c_str()); //define hostname
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) 
{
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;

    StaticJsonDocument<100> jsonReceived;
    deserializeJson(jsonReceived,(char*)data);
    
   // String myString= jsonReceived["Reply"];
   // testNumber= jsonReceived["Number"];
   // bool myBool = jsonReceived["Bool"];

    //Serial.println(myString + " " + testNumber + " " + myBool);
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() 
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
  Serial.println("Initialised the WebSocket");
}

void sendMessage()
{
    StaticJsonDocument<200> jsonSend;
    
    jsonSend["ip1"] = input1;
    jsonSend["ip2"] = input2;
    jsonSend["ip3"] = input3;
    jsonSend["ip4"] = input4;

    jsonSend["op1"] = output1;
    jsonSend["op2"] = output2;
    jsonSend["op3"] = output3;
    jsonSend["op4"] = output4;

    jsonSend["f1"] = function1clicked;
    jsonSend["f2"] = function2clicked;
    jsonSend["f3"] = function3clicked;
    jsonSend["f4"] = function4clicked;

    String strMessage = "";
    serializeJson(jsonSend,strMessage);
    ws.textAll(strMessage);
    Serial.println(strMessage);
}

void setup() 
{
  Serial.begin(115200);
  initSPIFFS();
  initWiFi();
  initWebSocket();

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  // Start server
  server.begin();
}

void pollingLoop()
{
    //get inputs
    //do logic
    //set outputs
}

void loop() 
{
static long lastMessage = 0;
  if(lastMessage<millis())
  {
      pollingLoop();
      sendMessage();
      lastMessage = millis() + 1000;
  }
  ws.cleanupClients();
}