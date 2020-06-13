#include <Arduino.h>


void initDisplay();
void beginNewText();
void beginUpdateText();
void endText();
String center(String s, int size);
String centerWithPadding(String s, int size, char pad);
void printOnLine(int line, const char *s);