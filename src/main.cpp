#include <Arduino.h>
#include "time.h"

#include "Pinout.h"
#include "Encoder.h"
#include "MyWebServer.h"

#define SAMPLE_COUNT 4000

void motor_start();
int pwm_value;

volatile bool start = false;

void setup() {
  Serial.begin(115200);
  MyWebServer::setup();
  Encoder::setup();
  configTime(-10800, 0, "pool.ntp.org");
}

void loop() {
  while (!start) continue;
  start = false;

  int left_buffer[SAMPLE_COUNT];
  int right_buffer[SAMPLE_COUNT];
  unsigned int time_buffer[SAMPLE_COUNT]; 

  struct tm timeinfo;
  getLocalTime(&timeinfo);
  char filename[40];
  sprintf(filename, "/pwm_%03d", pwm_value);
  strftime(filename + strlen(filename), 40, "_%T.csv", &timeinfo);

  unsigned long start_time = millis();
  const int DURATION = 800;
  const int SAMPLE_INTERVAL = 2;

  int saturation = min(pwm_value, 255 - pwm_value);

  float position = 0;
  // float speed = 1;
  // const float acc = 0.4;

  const float max_speed = 6;
  const float Kp = 4;

  // Motor
  Encoder::left_count = 0;
  Encoder::right_count = 0;
  motor_start();
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  

  for (int i = 0; i < 4000; i++)
  {
    int last_sample = millis();
    while (millis() - last_sample < SAMPLE_INTERVAL) continue;

    // int angle = Kp*(Encoder::right_count - Encoder::left_count);

    // angle = constrain(angle, -saturation, saturation);
    // analogWrite(MOT_PWML, pwm_value + angle);
    // analogWrite(MOT_PWMR, pwm_value - angle);

    time_buffer[i] = (int) (millis() - start_time);
    right_buffer[i] = Encoder::right_count;
    left_buffer[i] = Encoder::left_count;
  }
  analogWrite(MOT_PWML, 0);
  analogWrite(MOT_PWMR, 0);
  
  digitalWrite(2, LOW);
  
  File file = LittleFS.open(filename, FILE_WRITE, true);
  file.println("Time Left Right");


  
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
  // digitalWrite(MOT_AL, HIGH);
  // digitalWrite(MOT_BL, LOW);
  // digitalWrite(MOT_AR, HIGH);
  // digitalWrite(MOT_BR, LOW);
  // analogWrite(MOT_PWMR, pwm_value);
  // analogWrite(MOT_PWML, pwm_value);
}