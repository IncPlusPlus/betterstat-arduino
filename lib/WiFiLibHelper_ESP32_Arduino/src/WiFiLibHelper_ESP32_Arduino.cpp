#include "WiFiLibHelper.hpp"
#include <HttpClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

WiFiClient wifiClient = WiFiClient();
WiFiClientSecure wifiClientSecure = WiFiClientSecure();
bool certAlreadySet = false;

HttpClient getClient(const char *aServerName, uint16_t aServerPort, bool secure) {
  if (!certAlreadySet) {
    wifiClientSecure.setCACert(ca_cert);
    certAlreadySet = true;
  }
  return HttpClient(secure ? wifiClientSecure : wifiClient, aServerName, aServerPort);
}

HttpClient getClient(const String &aServerName, uint16_t aServerPort, bool secure) {
  if (!certAlreadySet) {
    wifiClientSecure.setCACert(ca_cert);
    certAlreadySet = true;
  }
  return HttpClient(secure ? wifiClientSecure : wifiClient, aServerName, aServerPort);
}

HttpClient getClient(const IPAddress &aServerAddress, uint16_t aServerPort, bool secure) {
  if (!certAlreadySet) {
    wifiClientSecure.setCACert(ca_cert);
    certAlreadySet = true;
  }
  return HttpClient(secure ? wifiClientSecure : wifiClient, aServerAddress, aServerPort);
}