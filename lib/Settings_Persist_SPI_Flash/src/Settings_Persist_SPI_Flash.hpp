#pragma once
#include "Settings_Persist.hpp"

bool initConfigStorage();

bool isSetUp();

void setSetUp(bool isSetUp);

void clearSetUp();

void putCreds(char *ssid, char *password);

struct WiFiCredsStruct getCreds();

void clearCreds();

bool credsExist();

void clearAllSettings();
