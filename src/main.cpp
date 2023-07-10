#include <Arduino.h>
#include "BluetoothSerial.h"

#define SAMPLE_INTERVAL 5
#define SAMPLE_COUNT (200/SAMPLE_INTERVAL)

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

void IRAM_ATTR left_encoder();
void IRAM_ATTR right_encoder();

volatile unsigned int left_count = 0;
volatile unsigned int right_count = 0;

void motor_setup();

void setup() {
  // Bluetooth
  BT.begin("Segredo_FF");
  // motor_setup();

  // Encoder
  pinMode(ENC_AL, INPUT);
  pinMode(ENC_AR, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_BL), left_encoder, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_BR), right_encoder, RISING);

}

void loop() {
  while (!BT.available()) continue;
  while (BT.available()) BT.read();

  // Motor
  
  BT.println("-");

  for (size_t i = 0; i < SAMPLE_COUNT; i++) {
    vTaskDelay(pdMS_TO_TICKS(SAMPLE_INTERVAL));
  }
  BT.println("-");
}

void IRAM_ATTR left_encoder() {
  if (digitalRead(ENC_BL)) left_count += 1;
  else left_count -= 1;
}

void IRAM_ATTR right_encoder() {
  if (digitalRead(ENC_BR)) right_count += 1;
  else right_count -= 1;
}


void motor_setup() {
  // pinMode(PIN_A, OUTPUT);
  // pinMode(PIN_B, OUTPUT);
  // pinMode(PWM_PIN, OUTPUT);
  // digitalWrite(PIN_A, HIGH);
  // digitalWrite(PIN_B, LOW);
  // digitalWrite(PWM_PIN, LOW);
}