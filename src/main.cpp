#include <Arduino.h>
#include <Display.h>
#include <Setup_Helper.h>

void setup() {
  initDisplay();
//  Uncomment this line, upload the program, comment it, and reupload to start with cleared credentials.
//clearCreds();
  Serial.begin(9600);
  runSetupIfNecessary();
}

void loop() {

}