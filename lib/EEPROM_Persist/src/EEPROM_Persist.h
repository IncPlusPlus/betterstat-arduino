#ifndef BETTERSTAT_ARDUINO_EEPROM_PERSIST_H
#define BETTERSTAT_ARDUINO_EEPROM_PERSIST_H

#include <WiFi101.h>

struct WiFiCreds {
  char ssid[M2M_MAX_SSID_LEN];
  char password[M2M_MAX_PSK_LEN];
};

void putCreds(char *ssid, char *password);

WiFiCreds getCreds();

#endif //BETTERSTAT_ARDUINO_EEPROM_PERSIST_H