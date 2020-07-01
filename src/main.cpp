#include <Arduino.h>
#include <Display.hpp>
#include <Setup_Helper.h>
//#include <Settings_Persist.hpp>
//#include <WiFiClient.h>
//Uncomment this line, upload the program, comment it, and reupload to start with cleared settings.
//#include <Settings_Persist.hpp>
//WiFiClient client;

void setup() {
  initDisplay();
//  Uncomment this line, upload the program, comment it, and reupload to start with cleared settings.
//  clearAllSettings();
  Serial.begin(115200);
  //Smuggle in the WiFiClient
  runSetupIfNecessary();
}

void loop() {
  Serial.println(F("In loop"));
  delay(5000);
}