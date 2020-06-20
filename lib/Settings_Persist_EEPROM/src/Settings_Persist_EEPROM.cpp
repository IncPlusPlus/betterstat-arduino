#include <Arduino.h>
#include <EEPROM.h>
#include "Settings_Persist_EEPROM.hpp"

const int isSetUpIndex = 0;
const int wifiCredsIndex = isSetUpIndex + sizeof(bool);

void clearEEPROM() {
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
}

bool isSetUp() {
  bool isSetUp;
  EEPROM.get(isSetUpIndex, isSetUp);
  return isSetUp;
}

void setSetUp(bool isSetUp) {
  EEPROM.put(isSetUpIndex, isSetUp);
}

void clearSetUp() {
  EEPROM.put(isSetUpIndex, false);
}

void putCreds(char *ssid, char *password) {
  WiFiCredsStruct creds;
  memcpy(creds.ssid, ssid, sizeof(creds.ssid));
  memcpy(creds.password, password, sizeof(creds.password));
  EEPROM.put(wifiCredsIndex, creds);
}
WiFiCredsStruct getCreds() {
  // EEPROM[0] will always be where WiFiCredsStruct is stored
  WiFiCredsStruct creds;
  EEPROM.get(wifiCredsIndex, creds);
  return creds;
}

void clearCreds() {
  for (int i = wifiCredsIndex; i < wifiCredsIndex + sizeof(WiFiCredsStruct); i++) {
    EEPROM.write(i, 0);
  }
}

bool credsExist() {
  WiFiCredsStruct creds = getCreds();
  bool credsValid = true;
  bool ssidEmpty = true;
  bool passwordEmpty = true;
//  Check if everything is just the number 0
  for (int i = 0; i < sizeof(creds.ssid); i++) {
    if (creds.ssid[i] != 0) {
      ssidEmpty = false;
      break;
    }
  }
  for (int i = 0; i < sizeof(creds.password); i++) {
    if (creds.password[i] != 0) {
      passwordEmpty = false;
      break;
    }
  }
//  The creds probably exist in EEPROM if the SSID and password aren't all 0s.
  return !ssidEmpty && !passwordEmpty;
}

void clearAllSettings() {
  clearSetUp();
  clearCreds();
}