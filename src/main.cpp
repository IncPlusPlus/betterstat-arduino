#include <Arduino.h>
#include <Display.hpp>
#include <Setup_Helper.h>
//Uncomment this line, upload the program, comment it, and reupload to start with cleared settings.
//#include <Settings_Persist.hpp>

void setup() {
  initDisplay();
//  Uncomment this line, upload the program, comment it, and reupload to start with cleared settings.
//  clearAllSettings();
  Serial.begin(115200);
  runSetupIfNecessary();
}

void loop() {
  Serial.println(F("In loop"));
  delay(5000);
}