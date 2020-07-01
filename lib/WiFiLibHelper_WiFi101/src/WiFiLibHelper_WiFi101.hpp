#pragma once

HttpClient getClient(const char* aServerName, uint16_t aServerPort, bool secure);

HttpClient getClient(const String& aServerName, uint16_t aServerPort, bool secure);

HttpClient getClient(const IPAddress& aServerAddress, uint16_t aServerPort, bool secure);