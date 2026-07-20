#include <Arduino.h>

#include <MD_MAX72xx.h>
#include <SPI.h>

#define CLK_PIN   14
#define DATA_PIN  27
#define CS_PIN    13

const int lModuleOffset = 0;
const int lCModuleOffset = 8;
const int rCModuleOffset = 16;
const int rModuleOffset = 24;

const int TOTAL_COLUMNS = 32;

MD_MAX72XX mx = MD_MAX72XX(MD_MAX72XX::FC16_HW, DATA_PIN, CLK_PIN, CS_PIN, 8);

bool isBraking = false;

enum TurnSignal {
  OFF,
  LEFT,
  HAZARD,
  RIGHT
};

TurnSignal turnSignal = OFF;

const uint8_t lArrow[4] = {
  0x18, 0x24, 0x42, 0x81
};

const uint8_t rArrow[4] = {
  0x81, 0x42, 0x24, 0x18
};

int lastDrawX = 0;
bool isDrawingTurnSignal = false;

void drawBitmap(const uint8_t* bmp, uint8_t width, int offset, bool overwrite) {
  for (uint8_t x = 0; x < width; x++) {
    int col = x + offset;
    if (overwrite) {
      if (col < 0 || col >= TOTAL_COLUMNS) continue;
      uint8_t current = mx.getColumn(col);
      mx.setColumn(col, current | bmp[x]);
    } else return;
    
  }
}

int leftArrowX = 0;
int rightArrowX = 0;

const int lTurnArrowXStart = 28;
const int rTurnArrowXStart = 0;

unsigned long lastTurnSignalArrowDraw = 0;
const int turnSignalDrawDelay = 20;

const int hazardSignalDrawDelay = 40;


const int lHazardArrowXStart = 12;
const int rHazardArrowXStart = 16;

int clubstep = 0; // somethng something geometry dash
const int hazardCycleLen = 13;

void updateDraw(TurnSignal signal, bool braking) {
  if (signal == OFF) {
    isDrawingTurnSignal = false;
    mx.clear();
    return;
  }

  else if (signal == LEFT)
  {
    if (millis() - lastTurnSignalArrowDraw < turnSignalDrawDelay) return;
    lastTurnSignalArrowDraw = millis(); 

    isDrawingTurnSignal = true;

    leftArrowX--;

    if (leftArrowX < 0) {
      leftArrowX = lTurnArrowXStart;
      mx.clear();
    }

    mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
    drawBitmap(lArrow, sizeof(lArrow), leftArrowX, true);
    mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  }
  
  else if (signal == RIGHT) {
    if (millis() - lastTurnSignalArrowDraw < turnSignalDrawDelay) return;
    lastTurnSignalArrowDraw = millis(); 

    isDrawingTurnSignal = true;

    rightArrowX++;

    if (rightArrowX > 28) {
      rightArrowX = rTurnArrowXStart;
      mx.clear();
    }

    mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
    drawBitmap(rArrow, sizeof(rArrow), rightArrowX, true);
    mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  }

  else if (signal == HAZARD) {
    if (millis() - lastTurnSignalArrowDraw < hazardSignalDrawDelay) return;
    lastTurnSignalArrowDraw = millis();

    clubstep++;
    if (clubstep >= hazardCycleLen) {
      clubstep = 0;
      mx.clear();
    }

    leftArrowX = lHazardArrowXStart - clubstep;
    rightArrowX = rHazardArrowXStart + clubstep;

    

    

    mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
    drawBitmap(lArrow, sizeof(lArrow), leftArrowX, true);
    drawBitmap(rArrow, sizeof(rArrow), rightArrowX, true);
    mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  }
}

void setup() {
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 1);

  isBraking = false;
  turnSignal = HAZARD; // debug
}

void loop() {
  updateDraw(turnSignal, isBraking);
  delay(10);
}