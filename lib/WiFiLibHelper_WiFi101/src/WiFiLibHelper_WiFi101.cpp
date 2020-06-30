#include <HttpClient.h>
#include <WiFi101.h>

WiFiClient wifiClient = WiFiClient();
WiFiSSLClient wiFiSSLClient = WiFiSSLClient();

HttpClient getClient(const char *aServerName, uint16_t aServerPort, bool secure) {
  return HttpClient(secure ? wiFiSSLClient : wifiClient, aServerName, aServerPort);
}

HttpClient getClient(const String &aServerName, uint16_t aServerPort, bool secure) {
  return HttpClient(secure ? wiFiSSLClient : wifiClient, aServerName, aServerPort);
}

HttpClient getClient(const IPAddress &aServerAddress, uint16_t aServerPort, bool secure) {
  return HttpClient(secure ? wiFiSSLClient : wifiClient, aServerAddress, aServerPort);
}