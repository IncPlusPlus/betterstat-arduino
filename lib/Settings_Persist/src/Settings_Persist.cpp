#include <Arduino.h>
#include <Serial_Utils.h>
#include <Settings_Persist.hpp>

WiFiCredsStruct promptForCreds() {
  WiFiCredsStruct userEnteredCreds;
  Serial.println(F("WiFi SSID: "));
  String ssid = readLine();
  ssid.toCharArray(userEnteredCreds.ssid, ssid.length());
  Serial.println(F("WiFi Password: "));
  String pass = readLine();
  pass.toCharArray(userEnteredCreds.password, pass.length());
  return userEnteredCreds;
}

ServerCredsStruct promptForServerCreds() {
  ServerCredsStruct userEnteredCreds;
  Serial.println(F("API username: "));
  String username = readLine();
  username.toCharArray(userEnteredCreds.username, username.length());
  Serial.println(F("API password: "));
  String pass = readLine();
  pass.toCharArray(userEnteredCreds.password, pass.length());
  return userEnteredCreds;
}

HostnameStruct promptForHostname() {
  HostnameStruct hostname_struct;
  Serial.println(F("Hostname: "));
  String hostnameString = readLine();
  Serial.println(F("Hostname is an IP: "));
  String boolString = readLine();
  Serial.println("Boolstring was");
  Serial.println(boolString);
  if (boolString == "true") {
    Serial.println("Setting to true");
    hostname_struct.isAnIP = true;
  } else {
    Serial.println("Setting to false");
    hostname_struct.isAnIP = false;
  }
  hostnameString.toCharArray(hostname_struct.hostname, hostnameString.length());
  return hostname_struct;
}
