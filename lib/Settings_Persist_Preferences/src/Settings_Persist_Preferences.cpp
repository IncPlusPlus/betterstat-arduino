#include "Settings_Persist_Preferences.hpp"
#include <Preferences.h>
#define NAMESPACE "betterstat"
#define SET_UP_BOOL_KEY "setUp"
#define WIFI_CREDS_KEY "WiFiCreds"
#define SERVER_CREDS_KEY "ServerCreds"
#define HOSTNAME_KEY "Hostname"

Preferences preferences;

bool initConfigStorage() {
  preferences.begin(NAMESPACE, false);
}

bool isSetUp() {
  return preferences.getBool(SET_UP_BOOL_KEY, false);
}

void setSetUp(bool isSetUp) {
  preferences.putBool(SET_UP_BOOL_KEY, isSetUp);
}

void clearSetUp() {
  preferences.remove(SET_UP_BOOL_KEY);
}

void putCreds(char *ssid, char *password) {
  WiFiCredsStruct creds;
  memcpy(creds.ssid, ssid, sizeof(creds.ssid));
  memcpy(creds.password, password, sizeof(creds.password));
  // force the struct into a byte array
  preferences.putBytes(WIFI_CREDS_KEY, &creds, sizeof(WiFiCredsStruct));
}

struct WiFiCredsStruct getCreds() {
  size_t credsLen = preferences.getBytesLength(WIFI_CREDS_KEY);
  char buffer[credsLen];
  preferences.getBytes(WIFI_CREDS_KEY, buffer, credsLen);
  return (WiFiCredsStruct &&) buffer;
}

void clearCreds() {
  preferences.remove(WIFI_CREDS_KEY);
}

bool credsExist() {
  //TODO: Do this, idiot.
  return false;
}

void putServerCreds(char *username, char *password) {
  ServerCredsStruct creds;
  memcpy(creds.username, username, sizeof(creds.username));
  memcpy(creds.password, password, sizeof(creds.password));
  // force the struct into a byte array
  preferences.putBytes(SERVER_CREDS_KEY, &creds, sizeof(WiFiCredsStruct));
}

struct ServerCredsStruct getServerCreds() {
  size_t credsLen = preferences.getBytesLength(SERVER_CREDS_KEY);
  char buffer[credsLen];
  preferences.getBytes(SERVER_CREDS_KEY, buffer, credsLen);
  return (ServerCredsStruct &&) buffer;
}

void clearServerCreds() {
  preferences.remove(SERVER_CREDS_KEY);
}

bool serverCredsExist() {
  //TODO: Do this, idiot.
  return false;
}

void putHostname(char *hostname, bool hostnameIsAnIP) {
  HostnameStruct hostname_struct;
  memcpy(hostname_struct.hostname, hostname, sizeof(hostname_struct.hostname));
  hostname_struct.isAnIP = hostnameIsAnIP;
  // force the struct into a byte array
  preferences.putBytes(HOSTNAME_KEY, &hostname_struct, sizeof(HostnameStruct));
}

struct HostnameStruct getHostname() {
  size_t hostnameLen = preferences.getBytesLength(HOSTNAME_KEY);
  char buffer[hostnameLen];
  preferences.getBytes(HOSTNAME_KEY, buffer, hostnameLen);
  return (HostnameStruct &&) buffer;
}

void clearHostname() {
  preferences.remove(HOSTNAME_KEY);
}

bool hostnameExists() {
  //TODO: Do this, idiot.
  return false;
}

void clearAllSettings() {
  preferences.clear();
}
