#ifndef BETTERSTAT_ARDUINO_DISPLAY_H
#define BETTERSTAT_ARDUINO_DISPLAY_H

#include <Arduino.h>

void initDisplay();

void beginNewText();

void beginUpdateText();

void endText();

String center(String s, int size);

String centerWithPadding(String s, int size, char pad);

void printOnLine(int line, const char *s);

#endif //BETTERSTAT_ARDUINO_DISPLAY_H