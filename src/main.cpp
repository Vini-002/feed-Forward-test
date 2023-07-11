#include <Arduino.h>
#include "BluetoothSerial.h"

#define SAMPLE_INTERVAL 4
#define SAMPLE_COUNT (800/SAMPLE_INTERVAL)

#define ENC_AR 22
#define ENC_BR 23

#define ENC_AL 17
#define ENC_BL 18

#define MOT_AR 14
#define MOT_BR 15
#define MOT_PWMR 19

#define MOT_AL 16
#define MOT_BL 5
#define MOT_PWML 21

BluetoothSerial BT;

void IRAM_ATTR left_A();
void IRAM_ATTR right_A();

void IRAM_ATTR left_B();
void IRAM_ATTR right_B();

volatile int left_count = 0;
volatile int right_count = 0;

void motor_start();

void setup() {
  // Bluetooth
  BT.begin("Segredo_FF");

  // Encoder
  attachInterrupt(digitalPinToInterrupt(ENC_AL), left_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_AR), right_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_BL), left_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_BR), right_B, CHANGE);
}

void loop() {
  while (!BT.available()) continue;
  while (BT.available()) BT.read();

  // Motor
  
  BT.println("-----\nLEFT, RIGHT");
  motor_start();

  for (size_t i = 0; i < SAMPLE_COUNT; i++) {
    BT.print(left_count);
    BT.print(" ");
    BT.println(right_count);
    vTaskDelay(pdMS_TO_TICKS(SAMPLE_INTERVAL));
  }
  analogWrite(MOT_PWML, 0);
  analogWrite(MOT_PWMR, 0);
  for (size_t i = 0; i < SAMPLE_COUNT; i++) {
    BT.print(left_count);
    BT.print(" ");
    BT.println(right_count);
    vTaskDelay(pdMS_TO_TICKS(SAMPLE_INTERVAL));
  }
  BT.println("-----");
}

void IRAM_ATTR left_A() {
  if (digitalRead(ENC_AL) != digitalRead(ENC_BL)) left_count += 1;
  else left_count -= 1;
}

void IRAM_ATTR right_A() {
  if (digitalRead(ENC_AR) != digitalRead(ENC_BR)) right_count += 1;
  else right_count -= 1;
}

void IRAM_ATTR left_B() {
  if (digitalRead(ENC_AL) == digitalRead(ENC_BL)) left_count += 1;
  else left_count -= 1;
}

void IRAM_ATTR right_B() {
  if (digitalRead(ENC_AR) == digitalRead(ENC_BR)) right_count += 1;
  else right_count -= 1;
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
  analogWrite(MOT_PWML, 140);
  analogWrite(MOT_PWMR, 140);
}