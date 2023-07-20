#pragma once
#include <Arduino.h>

#define ENC_AL 17
#define ENC_BL 18

#define ENC_AR 22
#define ENC_BR 23

namespace Encoder
{
  void IRAM_ATTR left_A();
  void IRAM_ATTR right_A();

  void IRAM_ATTR left_B();
  void IRAM_ATTR right_B();

  volatile int left_count = 0;
  volatile int right_count = 0;

  void setup();
};

void Encoder::setup()
{
  attachInterrupt(digitalPinToInterrupt(ENC_AL), left_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_AR), right_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_BL), left_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_BR), right_B, CHANGE);
}

// void IRAM_ATTR Encoder::left_A() {
//   if (digitalRead(ENC_AL) != digitalRead(ENC_BL)) left_count += 1;
//   else left_count -= 1;
// }

// void IRAM_ATTR Encoder::right_A() {
//   if (digitalRead(ENC_AR) != digitalRead(ENC_BR)) right_count += 1;
//   else right_count -= 1;
// }

// void IRAM_ATTR Encoder::left_B() {
//   if (digitalRead(ENC_AL) == digitalRead(ENC_BL)) left_count += 1;
//   else left_count -= 1;
// }

// void IRAM_ATTR Encoder::right_B() {
//   if (digitalRead(ENC_AR) == digitalRead(ENC_BR)) right_count += 1;
//   else right_count -= 1;
// }


void IRAM_ATTR Encoder::left_A() {
  if (((GPIO.in >> ENC_AL) & 0x01) != ((GPIO.in >> ENC_BL) & 0x01)) left_count += 1;
  else left_count -= 1;
}

void IRAM_ATTR Encoder::right_A() {
  if (((GPIO.in >> ENC_AR) & 0x01) != ((GPIO.in >> ENC_BR) & 0x01)) right_count += 1;
  else right_count -= 1;
}

void IRAM_ATTR Encoder::left_B() {
  if (((GPIO.in >> ENC_AL) & 0x01) == ((GPIO.in >> ENC_BL) & 0x01)) left_count += 1;
  else left_count -= 1;
}

void IRAM_ATTR Encoder::right_B() {
  if (((GPIO.in >> ENC_AR) & 0x01) == ((GPIO.in >> ENC_BR) & 0x01)) right_count += 1;
  else right_count -= 1;
}
