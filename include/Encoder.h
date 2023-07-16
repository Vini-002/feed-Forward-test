#pragma once
#include <Arduino.h>

namespace Encoder
{
  uint8_t _pin_al, _pin_bl, _pin_ar, _pin_br;

  void IRAM_ATTR left_A();
  void IRAM_ATTR right_A();

  void IRAM_ATTR left_B();
  void IRAM_ATTR right_B();

  volatile int left_count = 0;
  volatile int right_count = 0;

  void setup(uint8_t pin_al, uint8_t pin_bl, uint8_t pin_ar, uint8_t pin_br);
};

void Encoder::setup(uint8_t pin_al, uint8_t pin_bl, uint8_t pin_ar, uint8_t pin_br)
{
  _pin_al = pin_al;
  _pin_bl = pin_bl;
  _pin_ar = pin_ar;
  _pin_br = pin_br;
  attachInterrupt(digitalPinToInterrupt(_pin_al), left_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(_pin_ar), right_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(_pin_bl), left_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(_pin_br), right_B, CHANGE);
}


void IRAM_ATTR Encoder::left_A() {
  if ((GPIO.in >> _pin_al) ^ (GPIO.in >> _pin_bl) & 0x01) left_count += 1;
  else left_count -= 1;
}

void IRAM_ATTR Encoder::right_A() {
  if ((GPIO.in >> _pin_ar) ^ (GPIO.in >> _pin_br) & 0x01) right_count += 1;
  else right_count -= 1;
}

void IRAM_ATTR Encoder::left_B() {
  if ((GPIO.in >> _pin_al) ^ (GPIO.in >> _pin_bl) & 0x01) left_count -= 1;
  else left_count += 1;
}

void IRAM_ATTR Encoder::right_B() {
  if ((GPIO.in >> _pin_ar) ^ (GPIO.in >> _pin_br) & 0x01) right_count -= 1;
  else right_count += 1;
}
