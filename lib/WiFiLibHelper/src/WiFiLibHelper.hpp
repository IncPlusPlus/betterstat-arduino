#pragma once

#if WIFI_LIB_IN_USE == 1
#include "WiFiLibHelper_WiFi101.hpp"
#elif WIFI_LIB_IN_USE == 2
#include "WiFiLibHelper_ESP32_Arduino.hpp"
#else
#error "Please define WIFI_LIB_IN_USE with a valid value"
#endif

/**
 * This is a valid certificate for use with arduino-esp32's WiFiClientSecure class. Please see the
 * "Using a root certificate authority cert" section of the README.md present in
 * https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFiClientSecure.
 * Note: The ESP32 may not support the ciphers or signing method used to generate your site's SSL certificate.
 * It is much easier to never have your thermostat device need to communicate to your server outside the house
 * by simply assigning your server a static IP address and configuring the thermostat to talk to that IP.
 */
extern const char *ca_cert;