#include <Arduino.h>
#include <Serial_Utils.h>
#include "Settings_Persist.hpp"

WiFiCredsStruct promptForCreds() {
  WiFiCredsStruct userEnteredCreds;
  Serial.println(F("WiFi SSID: "));
  String ssid = readLine();
  ssid.toCharArray(userEnteredCreds.ssid, ssid.length());
  Serial.println(F("WiFi Password: "));
  String pass = readLine();
  pass.toCharArray(userEnteredCreds.password, pass.length());
  return userEnteredCreds;
}