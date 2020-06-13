#ifndef BETTERSTAT_ARDUINO_EEPROM_PERSIST_H
#define BETTERSTAT_ARDUINO_EEPROM_PERSIST_H

#include <WiFi101.h>

void clearEEPROM();

bool isSetUp();

void setSetUp(bool isSetUp);

struct WiFiCreds {
  char ssid[M2M_MAX_SSID_LEN];
  char password[M2M_MAX_PSK_LEN];
};

void putCreds(char *ssid, char *password);

WiFiCreds getCreds();

void clearCreds();

bool credsExist();

WiFiCreds promptForCreds();

#endif //BETTERSTAT_ARDUINO_EEPROM_PERSIST_H