#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ArduinoJSON.h>

const char* ssid = "SKYPEMHG";
const char* password = "8NHetSWQAJ75";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

int testNumber = 0;

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
    
    String myString= jsonReceived["Reply"];
    testNumber= jsonReceived["Number"];
    bool myBool = jsonReceived["Bool"];

    Serial.println(myString + " " + testNumber + " " + myBool);
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
    if (ws.count() == 0)
    {
      testNumber=0;
    }
    else{
      testNumber++;
    }
    StaticJsonDocument<100> jsonSend;
    jsonSend["Message"] = "From ESP32";
    jsonSend["Number"] = testNumber;
    jsonSend["Bool"] = false; 
    String strMessage = "";
    serializeJson(jsonSend,strMessage);
    ws.textAll(strMessage);
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

void loop() 
{
static long lastMessage = 0;
  if(lastMessage<millis())
  {
      sendMessage();
      lastMessage = millis() + 1000;
  }
  ws.cleanupClients();
}