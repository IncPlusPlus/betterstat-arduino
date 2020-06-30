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

void putServerCreds(char *username, char *password);

struct ServerCredsStruct getServerCreds();

void clearServerCreds();

bool serverCredsExist();

void putHostname(bool hostnameIsAnIP, bool isSecure, uint16_t port, char *hostname);

struct HostnameStruct getHostname();

void clearHostname();

bool hostnameExists();

void clearAllSettings();
