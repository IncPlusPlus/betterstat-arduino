#pragma once
#include "Settings_Persist.hpp"

bool initConfigStorage();

void clearEEPROM();

bool isSetUp();

void setSetUp(bool isSetUp);

void clearSetUp();

//struct WiFiCredsStruct {
//  char ssid[33];
//  char password[65];
//};

void putCreds(char *ssid, char *password);

struct WiFiCredsStruct getCreds();

void clearCreds();

bool credsExist();

void clearAllSettings();
