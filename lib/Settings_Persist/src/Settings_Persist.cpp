#include <Arduino.h>
#include <Serial_Utils.h>
#include <Settings_Persist.hpp>

WiFiCredsStruct promptForCreds() {
  WiFiCredsStruct userEnteredCreds;
  Serial.println(F("WiFi SSID: "));
  String ssid = readLine();
  ssid.trim();
  ssid.toCharArray(userEnteredCreds.ssid, sizeof(userEnteredCreds.ssid));
  Serial.println(F("WiFi Password: "));
  String pass = readLine();
  pass.trim();
  pass.toCharArray(userEnteredCreds.password, sizeof(userEnteredCreds.password));
  return userEnteredCreds;
}

ServerCredsStruct promptForServerCreds() {
  ServerCredsStruct userEnteredCreds;
  Serial.println(F("API username: "));
  String username = readLine();
  username.toCharArray(userEnteredCreds.username, sizeof(userEnteredCreds.username));
  Serial.println(F("API password: "));
  String pass = readLine();
  pass.toCharArray(userEnteredCreds.password, sizeof(userEnteredCreds.password));
  return userEnteredCreds;
}

HostnameStruct promptForHostname() {
  HostnameStruct hostname_struct;
  Serial.println(F("Hostname is an IP: "));
  String isAnIP = readLine();
  isAnIP.trim();
  Serial.println(F("Is secure: "));
  String isSecure = readLine();
  isSecure.trim();
  Serial.println(F("Port: "));
  String port = readLine();
  port.trim();
  Serial.println(F("Hostname: "));
  String hostnameString = readLine();
  hostnameString.trim();
  if (isAnIP == "true") {
    hostname_struct.isAnIP = true;
  } else {
    hostname_struct.isAnIP = false;
  }
  if (isSecure == "true") {
    hostname_struct.isSecure = true;
  } else { hostname_struct.isSecure = false; }
  hostname_struct.port = port.toInt();
  hostnameString.toCharArray(hostname_struct.hostname, sizeof(hostname_struct.hostname));
  return hostname_struct;
}
