#include <WiFi101.h>
#include <Settings_Persist.hpp>
#include <Display.h>
#include <avr/pgmspace.h>

const int setupStepTransitionDelay = 2500;
const PROGMEM String clearString = "                    ";

void wifiCredsSetup() {
  lcd.setCursor(0, 2);
  lcd.print(F("Credentials required"));
  delay(setupStepTransitionDelay);

  lcd.setCursor(0, 2);
  lcd.print(F("Reading terminal.   "));
  lcd.setCursor(0, 3);
  lcd.print(F("Waiting for input..."));

  WiFiCredsStruct userEnteredCreds = promptForCreds();
  putCreds(userEnteredCreds.ssid, userEnteredCreds.password);

  lcd.setCursor(0, 2);
  lcd.print(clearString);
  lcd.setCursor(0, 2);
  lcd.print(F("Received credentials"));
//  lcd.setCursor(0,3);
//  lcd.print(clearString);

}

char const *getWl_status_t(int status) {
  switch (status) {
    case WL_NO_SHIELD:return "no shield";
    case WL_IDLE_STATUS:return "idle";
    case WL_NO_SSID_AVAIL:return "no SSID avail";
    case WL_SCAN_COMPLETED:return "scan complete";
    case WL_CONNECTED:return "connected";
    case WL_CONNECT_FAILED:return "connect failed";
    case WL_CONNECTION_LOST:return "connection lost";
    case WL_DISCONNECTED:return "disconnected";
    case WL_AP_LISTENING:return "AP listening";
    case WL_AP_CONNECTED:return "AP connected";
    case WL_AP_FAILED:return "AP failed";
    case WL_PROVISIONING:return "WL provisioning";
    case WL_PROVISIONING_FAILED:return "WL provis failed";
    default:return "unknown status";
  }
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}

void printWiFiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);

}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void wifiConnect() {
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {

    lcd.setCursor(0, 2);
    lcd.print(F("SETUP FAILED!!!"));
    lcd.setCursor(0, 3);
    lcd.print(F("WiFi shield missing!"));

    // don't continue:
    // TODO: Talk back to the server here if setup is happening so the user can see the issue in-app.
    // while (true);
  }
  Serial.println(F("About to connect"));
  int status = WL_IDLE_STATUS;     // the WiFi radio's status

  lcd.setCursor(0, 2);
  lcd.print(clearString);
  lcd.setCursor(0, 2);
  lcd.print(F("Status:"));
  lcd.setCursor(0, 3);
  lcd.print(clearString);
  lcd.setCursor(0, 3);
  lcd.print(getWl_status_t(status));
  delay(setupStepTransitionDelay);
  while (status != WL_CONNECTED) {
    // Connect to WPA/WPA2 network:
    WiFiCredsStruct creds = getCreds();
    Serial.println(F("Grabbed creds. Connecting"));
    Serial.println(creds.ssid);
    Serial.println(creds.password);
    status = WiFi.begin(creds.ssid, creds.password);
    Serial.println(F("Finished WiFi.begin() call"));
    lcd.setCursor(0, 3);
    lcd.print(getWl_status_t(status));
    // wait 10 seconds for connection:
    delay(setupStepTransitionDelay);
    Serial.println(getWl_status_t(status));
    lcd.setCursor(0, 3);
    lcd.print(getWl_status_t(WiFi.status()));
  }
  printCurrentNet();
  printWiFiData();
}

void doTokenSetup() {
  lcd.setCursor(0, 2);
  lcd.print(clearString);
  lcd.setCursor(0, 3);
  lcd.print(clearString);

}

void doServerHostnameOrIpSetup() {
  lcd.setCursor(0, 2);
  lcd.print(clearString);
  lcd.setCursor(0, 3);
  lcd.print(clearString);

}

void confirmServerCommunication() {
  lcd.setCursor(0, 2);
  lcd.print(clearString);
  lcd.setCursor(0, 3);
  lcd.print(clearString);

}

void finalizeSetup() {
  // TODO: Send message to the server to have the app prompt the user to do the next setup steps.
  setSetUp(true);
}

///Sets up this thermostat to be able to talk to the betterstat server.
///Delays exist in this function so that the user can follow along.
void setupThermostat() {
  //<editor-fold desc="Set up WiFi credentials">

  lcd.setCursor(0, 0);
  lcd.print(F("Running setup (1/5)"));
  lcd.setCursor(0, 1);
  lcd.print(F("Setting up: WiFi"));

  wifiCredsSetup();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Connect to WiFi">
  lcd.setCursor(0, 0);
  lcd.print(F("Running setup (2/5)"));
  lcd.setCursor(0, 1);
  lcd.print(F("Connecting to WiFi"));

  wifiConnect();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Get auth token from server">
  lcd.setCursor(0, 0);
  lcd.print(F("Running setup (3/5)"));
  lcd.setCursor(0, 1);
  lcd.print(F("Getting auth token"));

//  TODO: We don't have this set up server-side yet. Therefore, there's no Arduino implementation for it yet either.
  doTokenSetup();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Save hostname or IP to EEPROM">
  lcd.setCursor(0, 0);
  lcd.print(F("Running setup (4/5)"));
  lcd.setCursor(0, 1);
  lcd.print(F("Saving host IP/name"));

//  TODO: We don't have this set up server-side yet. Therefore, there's no Arduino implementation for it yet either.
  doServerHostnameOrIpSetup();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Test communication with server">
  lcd.setCursor(0, 0);
  lcd.print(F("Running setup (5/5)"));
  lcd.setCursor(0, 1);
  lcd.print(F("Talking to server  "));

//  TODO: We don't have this set up server-side yet. Therefore, there's no Arduino implementation for it yet either.
  confirmServerCommunication();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Setup complete">
  lcd.setCursor(0, 0);
  lcd.print(F("Setup complete!    "));
  lcd.setCursor(0, 1);
  lcd.print(F("Please look at the "));
  lcd.setCursor(0, 2);
  lcd.print(F("app for next steps."));
  lcd.setCursor(0, 3);
  lcd.print(clearString);

  finalizeSetup();
  while (true);
  //</editor-fold>
}

void startThermostat() {
  //<editor-fold desc="Connect to WiFi">
  lcd.setCursor(0, 0);
  lcd.print(F("Starting up (1/2)"));
  lcd.setCursor(0, 1);
  lcd.print(F("Connecting to WiFi"));
  wifiConnect();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Test communication with server">
  lcd.setCursor(0, 0);
  lcd.print(F("Starting up (2/2)"));
  lcd.setCursor(0, 1);
  lcd.print(F("Talking to server "));
//  TODO: We don't have this set up server-side yet. Therefore, there's no Arduino implementation for it yet either.
//  confirmServerCommunication();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Startup complete">
  lcd.setCursor(0, 0);
  lcd.print(F("Startup complete!"));
  lcd.setCursor(0, 1);
  lcd.print(clearString);
  delay(setupStepTransitionDelay);
  //</editor-fold>
}

void runSetupIfNecessary() {
  initConfigStorage();
  if (!isSetUp())
    setupThermostat();
  startThermostat();
}