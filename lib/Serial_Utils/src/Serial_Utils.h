#pragma once
#include <Arduino.h>
#include <IPAddress.h>

String readLine();

String getDelimitedString(const String &s);

String ipAddressToString(IPAddress &ip);

String ipAddressToString(uint32_t ip);
