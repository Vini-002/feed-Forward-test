#include <Arduino.h>
#include "time.h"

#include "Pinout.h"
#include "Encoder.h"
#include "MyWebServer.h"

#define ON_TIME 300
#define OFF_TIME 0

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -3*3600;
const int   daylightOffset_sec = 0;

void motor_start();
int pwm_value;

volatile bool start = false;

void setup() {
  Serial.begin(115200);
  MyWebServer::setup();
  Encoder::setup();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  while (!start) continue;
  start = false;

  int left_buffer[200];
  int right_buffer[200];
  unsigned int time_buffer[200]; 

  struct tm timeinfo;
  getLocalTime(&timeinfo);
  char filename[40];
  sprintf(filename, "/testes/pwm_%03d", pwm_value);
  strftime(filename + strlen(filename), 40, "_%T.csv", &timeinfo);

  unsigned long start_time = millis();
  const int DURATION = 800;
  const int SAMPLE_INTERVAL = 4;

  int saturation = min(pwm_value, 255 - pwm_value);

  float position = 0;
  float speed = 1;
  const float acc = 0.4;

  const float max_speed = 10;

  // Motor
  Encoder::left_count = 0;
  Encoder::right_count = 0;
  motor_start();

  for (size_t i = 0; i < 200; i++)
  {
    int last_sample = millis();
    while (millis() - last_sample < SAMPLE_INTERVAL) continue;

    position += speed;
    speed = min((speed + acc), max_speed);
    
    float left_error = position - Encoder::left_count;
    float right_error = position - Encoder::right_count;

    // int angle = Encoder::right_count - Encoder::left_count;

    // angle = constrain(angle, -saturation, saturation);
    analogWrite(MOT_PWML, 50 + left_error);
    analogWrite(MOT_PWMR, 50 + right_error);

    // file.printf("%d %d\n", Encoder::left_count, Encoder::right_count);
    time_buffer[i] = (int) (millis() - start_time);
    right_buffer[i] = Encoder::right_count;
    left_buffer[i] = Encoder::left_count;
  }
  analogWrite(MOT_PWML, 0);
  analogWrite(MOT_PWMR, 0);

  File file = LittleFS.open(filename, FILE_WRITE, true);
  file.println("Time Left Right");

  for (size_t i = 0; i < 200; i++) {
    file.printf("%5d %5d %5d\n", time_buffer[i], left_buffer[i], right_buffer[i]);
  }
  
  File exp_list = LittleFS.open("/exp_list.txt", FILE_APPEND);
  exp_list.println(file.name());
  exp_list.close();
  file.close();
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
  analogWrite(MOT_PWML, pwm_value);
  analogWrite(MOT_PWMR, pwm_value);
}