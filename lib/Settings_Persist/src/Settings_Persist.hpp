#pragma once

#if (PERSISTENT_STORAGE_TYPE == 1)
//#define BETTERSTAT_SETTINGS_PERSIST_EEPROM
#include "Settings_Persist_EEPROM.hpp"
#elif (PERSISTENT_STORAGE_TYPE == 2)
#include "Settings_Persist_SPI_Flash.hpp"
//#define BETTERSTAT_SETTINGS_PERSIST_SPI_FLASH
#else
#error "Plesae define PERSISTENT_STORAGE_TYPE with a valid value"
#endif

struct WiFiCredsStruct {
  char ssid[33];
  char password[65];
};

WiFiCredsStruct promptForCreds();