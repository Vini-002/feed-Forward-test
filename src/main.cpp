#include <Arduino.h>
#include "BluetoothSerial.h"

#define SAMPLE_INTERVAL 5
#define SAMPLE_COUNT (200/SAMPLE_INTERVAL)
#define AVG_FILTER 8

// #define ENC_A_PIN 27
// #define ENC_B_PIN 26

#define PIN_A 16
#define PIN_B 4
#define PWM_PIN 2
#define ANALOG_PIN 35

BluetoothSerial BT;

// void IRAM_ATTR encoder();
// volatile unsigned int encoder_count = 0;

void motor_setup();

void setup() {
  // Bluetooth
  BT.begin("Segredo_FF");
  motor_setup();
  analogSetClockDiv(8);
  analogSetAttenuation(ADC_6db);

  // ledcSetup(0, 10000, 10);
  // ledcAttachPin(2, 0);
}

void loop() {
  while (!BT.available()) {
    int avg = 0;
    for (size_t i = 0; i < AVG_FILTER; i++)
    {
      avg += analogRead(ANALOG_PIN);
    }
    BT.println(avg);
    vTaskDelay(1000);
  }
  while (BT.available()) BT.read();

  // Encoder
  // pinMode(ENC_B_PIN, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(ENC_A_PIN), encoder, RISING);

  // Motor
  
  BT.println("-");
  // digitalWrite(PWM_PIN, HIGH);
  analogWrite(PWM_PIN, 100);

  for (size_t i = 0; i < SAMPLE_COUNT; i++) {
    int avg = 0;
    for (size_t i = 0; i < AVG_FILTER; i++)
    {
      avg += analogRead(ANALOG_PIN);
    }
    BT.println(avg);
    vTaskDelay(pdMS_TO_TICKS(SAMPLE_INTERVAL));
  }
  analogWrite(PWM_PIN, 0);
  // digitalWrite(PWM_PIN, LOW);
  BT.println("-");
}

// void IRAM_ATTR encoder() {
//   if (digitalRead(ENC_B_PIN)) encoder_count += 1;
//   else encoder_count -= 1;
// }

void motor_setup() {
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, LOW);
  digitalWrite(PWM_PIN, LOW);
}