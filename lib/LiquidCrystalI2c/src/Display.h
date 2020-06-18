#ifndef BETTERSTAT_ARDUINO_DISPLAY_H
#define BETTERSTAT_ARDUINO_DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "display_variables.h"

void initDisplay();

extern LiquidCrystal_I2C lcd;

#endif //BETTERSTAT_ARDUINO_DISPLAY_H