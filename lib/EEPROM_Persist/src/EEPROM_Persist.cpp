#include <Arduino.h>
#include <EEPROM.h>
#include <EEPROM_Persist.h>

void putCreds(char *ssid, char *password) {
  WiFiCreds creds;
  memcpy(creds.ssid, ssid, sizeof(creds.ssid));
  memcpy(creds.password, password, sizeof(creds.password));
  // EEPROM[0] will always be where WiFiCreds is stored
  EEPROM.put(0, creds);
}
WiFiCreds getCreds() {
  // EEPROM[0] will always be where WiFiCreds is stored
  WiFiCreds creds;
  EEPROM.get(0, creds);
  return creds;
}