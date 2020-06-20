#include <Arduino.h>
#include <Display.h>
#include <Setup_Helper.h>
//Uncomment this line, upload the program, comment it, and reupload to start with cleared settings.
//#include <Settings_Persist.hpp>

void setup() {
  lcd.init();
  lcd.backlight();
//  Uncomment this line, upload the program, comment it, and reupload to start with cleared settings.
// setSetUp(false);
  Serial.begin(9600);
  runSetupIfNecessary();
}

void loop() {
  Serial.println(F("In loop"));
  delay(5000);
}