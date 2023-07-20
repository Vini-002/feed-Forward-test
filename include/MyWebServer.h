#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <LittleFS.h>

#include "WifiCredentials.h"

extern const char* ssid;
extern const char* password;

extern volatile bool start;
extern int pwm_value;

namespace MyWebServer
{
  AsyncWebServer server(80);
  String sliderValue = "0";
  const char* PARAM_INPUT = "value";
  String processor(const String& var);
  void handleFileRequest(AsyncWebServerRequest *request);

  void setup();

  void restart();
} // namespace MyWebServer

void MyWebServer::setup() {
  LittleFS.begin();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());

  restart();
}


void MyWebServer::restart() {

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/style.css", "text/css");
  });

  server.on("/start", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "OK");
    start = true;
  });

  server.on("/view", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/exp_list.txt");
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      pwm_value = sliderValue.toInt();
    }
    else {
      inputMessage = "No message sent";
    }
    request->send(200, "text/plain", "OK");
  });

  server.onNotFound([](AsyncWebServerRequest *request) {
    handleFileRequest(request);
  });

  server.begin();
}

// Function to handle file requests
void MyWebServer::handleFileRequest(AsyncWebServerRequest *request) {
  String filename = request->url();
  // filename.remove(0, 1); // Remove the leading '/' from the filename

  // Check if the file exists in the LittleFS
  if (LittleFS.exists(filename)) {
    // If the file exists, send it as the response
    request->send(LittleFS, filename, "text/plain");
  } else {
    // If the file doesn't exist, return a 404 Not Found error
    request->send(404, "text/plain", "File not found");
  }
}

String MyWebServer::processor(const String& var) {
  //Serial.println(var);
  if (var == "SLIDERVALUE"){
    return sliderValue;
  }
  return String();
}
