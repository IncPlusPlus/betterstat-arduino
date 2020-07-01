#pragma once
#include <stdint.h>

#if PERSISTENT_STORAGE_TYPE == 1
//#define BETTERSTAT_SETTINGS_PERSIST_EEPROM
#include "Settings_Persist_EEPROM.hpp"
#elif PERSISTENT_STORAGE_TYPE == 2
#include "Settings_Persist_SPI_Flash.hpp"
//#define BETTERSTAT_SETTINGS_PERSIST_SPI_FLASH
#elif PERSISTENT_STORAGE_TYPE == 3
#include "Settings_Persist_Preferences.hpp"
//#define BETTERSTAT_SETTINGS_PERSIST_PREFERENCES
#else
#error "Please define PERSISTENT_STORAGE_TYPE with a valid value"
#endif

/*
 * You'll notice a lot of char arrays whose size are n+1.
 * I do this to keep my sanity because a string with 32 characters needs
 * a length 33 char array. I don't just write 33 because I want to very explicitly
 * know how much of the array is useful and whether or not I've remembered
 * to account for that damn null byte.
 */

struct WiFiCredsStruct {
  char ssid[33 + 1];
  char password[65 + 1];
};

struct ServerCredsStruct {
  char username[32 + 1];
  char password[32 + 1];
};

struct HostnameStruct {
  bool isAnIP;
  bool isSecure;
  uint16_t port;
  char hostname[64 + 1];
};

WiFiCredsStruct promptForCreds();
ServerCredsStruct promptForServerCreds();
HostnameStruct promptForHostname();
