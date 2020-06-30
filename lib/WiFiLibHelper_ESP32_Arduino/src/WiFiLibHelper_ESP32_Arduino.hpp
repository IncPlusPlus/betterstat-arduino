#pragma once
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <HttpClient.h>

HttpClient getClient(const char *aServerName, uint16_t aServerPort, bool secure);

HttpClient getClient(const String &aServerName, uint16_t aServerPort, bool secure);

HttpClient getClient(const IPAddress &aServerAddress, uint16_t aServerPort, bool secure);