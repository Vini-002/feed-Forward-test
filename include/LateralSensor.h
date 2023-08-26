#pragma once
#include <Arduino.h>

// ESQUERDA
#define SNSL0 36
#define SNSL1 34

// DIREITA
#define SNSR0 4 // ESSE PINO FODE OS DEMAIS
#define SNSR1 35


namespace LateralSensor
{
  
} // namespace LateralSensor

void IRAM_ATTR left();
volatile unsigned int left_count = 0;
volatile unsigned long last_left = 0;

void IRAM_ATTR right();
volatile unsigned int right_count = 0;
volatile unsigned long last_right = 0;

void setup() {
  attachInterrupt(digitalPinToInterrupt(SNSL0), left, RISING);
  attachInterrupt(digitalPinToInterrupt(SNSL1), left, RISING);
  attachInterrupt(digitalPinToInterrupt(SNSR1), right, RISING);
}

void IRAM_ATTR left() {
  if (millis() - last_left < 600) return;
  left_count ++;
  last_left = millis();
}

void IRAM_ATTR right() {
  if (millis() - last_right < 600) return;
  right_count ++;
  last_right = millis();
}
