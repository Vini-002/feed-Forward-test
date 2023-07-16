#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#include "WifiCredentials.h"
#include "Pinout.h"

#include "Encoder.h"

#define ON_TIME 300
#define OFF_TIME 0

AsyncWebServer server(80);

void motor_start();

volatile bool start = false;

void setup() {

  Serial.begin(115200);


  #include "esp_timer.h"

  volatile int test = 65530;
  volatile float testando;

  const unsigned MEASUREMENTS = 5000000;
  uint64_t start_t = esp_timer_get_time();

  for (int retries = 0; retries < MEASUREMENTS; retries++) {
    testando = 500.f/test; 
  }

  uint64_t end = esp_timer_get_time();

  Serial.printf("%u iterations took %llu milliseconds (%llu microseconds per invocation)\nResult: %.15f",
          MEASUREMENTS, (end - start_t)/1000, (end - start_t)/MEASUREMENTS, testando);

  SPIFFS.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/start", HTTP_GET, [](AsyncWebServerRequest *request){
    start = true;
    request->send(SPIFFS, "/index.html");
  });

  server.on("/view", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/teste.txt");
  });
  
  server.begin();
}

void loop() {

  while (!start) continue;

  Encoder::start_time = micros();

  // Motor
  motor_start();
  vTaskDelay(pdMS_TO_TICKS(ON_TIME));
  analogWrite(MOT_PWML, 0);
  analogWrite(MOT_PWMR, 0);
  vTaskDelay(pdMS_TO_TICKS(OFF_TIME));

  File file = SPIFFS.open("/teste.txt", FILE_WRITE);
  file.println("LEFT RIGHT");
  for (size_t i = 0; i < BUFFER_SIZE; i++) {
    if (Encoder::left_times[i] == 0 && Encoder::right_times[i] == 0) break;

    file.printf("%li %li\n", Encoder::left_times[i], Encoder::right_times[i]);
  }
  
  file.close();

  start = false;
}

void motor_start() {
  pinMode(MOT_AL, OUTPUT);
  pinMode(MOT_BL, OUTPUT);
  pinMode(MOT_PWML, OUTPUT);
  pinMode(MOT_AR, OUTPUT);
  pinMode(MOT_BR, OUTPUT);
  pinMode(MOT_PWMR, OUTPUT);
  digitalWrite(MOT_AL, HIGH);
  digitalWrite(MOT_BL, LOW);
  digitalWrite(MOT_AR, HIGH);
  digitalWrite(MOT_BR, LOW);
  analogWrite(MOT_PWML, 100);
  analogWrite(MOT_PWMR, 100);
}