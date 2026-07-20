#include <Arduino.h>

#include <MD_MAX72xx.h>
#include <SPI.h>

#define CLK_PIN   14
#define DATA_PIN  27
#define CS_PIN    13

MD_MAX72XX mx = MD_MAX72XX(MD_MAX72XX::FC16_HW, DATA_PIN, CLK_PIN, CS_PIN, 8);

bool g_isBraking = false;
bool g_isTurnSignal = false;

enum turnSignal {
  OFF,
  LEFT,
  HAZARD,
  RIGHT
};


void updateDraw(bool blinking, bool braking, int direction) {
  if (g_isBraking) {

  }
}

void setup() {
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 1);

  g_isBraking = false;
  g_isTurnSignal = false;
}

void loop() {
  delay(10);
}