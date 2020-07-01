#pragma once
#include <Adafruit_SSD1306.h>

void initDisplay();

extern Adafruit_SSD1306 lcd;

void clearDisplay();

void setDisplayCursor(uint8_t col, uint8_t row);

void flushDisplay();