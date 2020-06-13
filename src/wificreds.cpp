#include <Arduino.h>
#include <EEPROM.h>
#include <WiFi101.h>

struct WiFiCreds
{
  char ssid[M2M_MAX_SSID_LEN];
  char password[M2M_MAX_PSK_LEN];
};

void putCreds(char *ssid, char *password);
WiFiCreds getCreds();

void putCreds(char *ssid, char *password)
{
  WiFiCreds creds;
  memcpy(creds.ssid, ssid, sizeof(creds.ssid));
  memcpy(creds.password, password, sizeof(creds.password));
  // EEPROM[0] will always be where WiFiCreds is stored
  EEPROM.put(0, creds);
}
WiFiCreds getCreds()
{
  // EEPROM[0] will always be where WiFiCreds is stored
  WiFiCreds creds;
  EEPROM.get(0, creds);
  return creds;
}