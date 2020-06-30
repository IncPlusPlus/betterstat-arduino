#pragma once
#include <LiquidCrystal_I2C.h>

void initDisplay();

extern LiquidCrystal_I2C lcd;

void clearDisplay();

void setDisplayCursor(uint8_t col, uint8_t row);

void flushDisplay();