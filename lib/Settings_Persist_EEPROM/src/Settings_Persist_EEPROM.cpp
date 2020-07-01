#include <Arduino.h>
#include <EEPROM.h>
#include "Settings_Persist_EEPROM.hpp"

const int isSetUpIndex = 0;
const int wifiCredsIndex = isSetUpIndex + sizeof(bool);
const int serverCredsIndex = wifiCredsIndex + sizeof(WiFiCredsStruct);
const int hostnameIndex = serverCredsIndex + sizeof(ServerCredsStruct);

bool initConfigStorage() {
  return true;
  //Does nothing
}

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

void putServerCreds(char *username, char *password) {
  ServerCredsStruct creds;
  memcpy(creds.username, username, sizeof(creds.username));
  memcpy(creds.password, password, sizeof(creds.password));
  EEPROM.put(serverCredsIndex, creds);
}

struct ServerCredsStruct getServerCreds() {
  ServerCredsStruct creds;
  EEPROM.get(serverCredsIndex, creds);
  return creds;
}

void clearServerCreds() {
  for (int i = serverCredsIndex; i < serverCredsIndex + sizeof(ServerCredsStruct); i++) {
    EEPROM.write(i, 0);
  }
}

bool serverCredsExist() {
  ServerCredsStruct creds = getServerCreds();
  bool ssidEmpty = true;
  bool passwordEmpty = true;
//  Check if everything is just the number 0
  for (int i = 0; i < sizeof(creds.username); i++) {
    if (creds.username[i] != 0) {
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
//  The creds probably exist in EEPROM if the username and password aren't all 0s.
  return !ssidEmpty && !passwordEmpty;
}

void putHostname(bool hostnameIsAnIP, bool isSecure, uint16_t port, char *hostname) {
  HostnameStruct hostname_struct;
  hostname_struct.isAnIP = hostnameIsAnIP;
  hostname_struct.isSecure = isSecure;
  hostname_struct.port = port;
  memcpy(hostname_struct.hostname, hostname, sizeof(hostname_struct.hostname));
  EEPROM.put(hostnameIndex, hostname_struct);
}

struct HostnameStruct getHostname() {
  HostnameStruct hostname;
  EEPROM.get(hostnameIndex, hostname);
  return hostname;
}

void clearHostname() {
  for (int i = hostnameIndex; i < hostnameIndex + sizeof(HostnameStruct); i++) {
    EEPROM.write(i, 0);
  }
}

bool hostnameExists() {
  HostnameStruct hostname_struct = getHostname();
  bool usernameEmpty = true;
//  Check if everything is just the number 0
  for (int i = 0; i < sizeof(hostname_struct.hostname); i++) {
    if (hostname_struct.hostname[i] != 0) {
      usernameEmpty = false;
      break;
    }
  }
//  The hostname_struct probably exist in EEPROM if the hostname isn't all 0s.
  return !usernameEmpty;
}

void clearAllSettings() {
  clearSetUp();
  clearCreds();
  clearServerCreds();
  clearHostname();
}