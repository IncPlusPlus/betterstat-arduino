#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <WiFiHelper.h>
#include <Settings_Persist.hpp>
#include <Display.hpp>
#include <HttpClient.h>
#include <WiFiLibHelper.hpp>
#include <base64.h>
#include <Serial_Utils.h>

const int setupStepTransitionDelay = 5000;
const int setupWaitTimeSecs = 20;
const PROGMEM String clearString = "                    ";

void printCheckingForInputSeconds(int secs) {
  clearDisplay();
  setDisplayCursor(0, 0);
  lcd.print(F("Checking for"));
  setDisplayCursor(0, 1);
  lcd.print(F("serial input"));
  setDisplayCursor(0, 2);
  lcd.print(secs);
  lcd.print(F(" seconds...   "));
  flushDisplay();
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
//    case WL_AP_LISTENING:return "AP listening";
//    case WL_AP_CONNECTED:return "AP connected";
//    case WL_AP_FAILED:return "AP failed";
//    case WL_PROVISIONING:return "WL provisioning";
//    case WL_PROVISIONING_FAILED:return "WL provis failed";
    default:return "unknown status";
  }
}

/**
 * Call this if the setup fails at any point. This will notify the server
 * of the failure with the specified reason. This, in turn, will allow the user
 * to see the error and reason in the app.
 *
 * This method is an activity sink. It will throw the device into an endless while(true){}.
 * Call it if there is no way to recover gracefully.
 * @param reason the reason or details about the failure
 */
[[noreturn]] void failSetup(const char *reason) {
  clearDisplay();
  setDisplayCursor(0, 0);
  lcd.print(F("SETUP FAILED!!!"));
  setDisplayCursor(0, 1);
  lcd.print(F("Please check the app"));
  flushDisplay();
  Serial.print(F("SETUP_FAILED"));
  /*
   * We don't always know what the reason parameter will contain. If it contains newlines,
   * the server's reading methods will stop reading once that is seen. To smuggle those through,
   * a base64 encoded string is passed to the server.
   */
  Serial.println(base64::encode(reason));
  //Hang indefinitely
  while (true) {}
}

/**
 * @see failSetup(const char *reason)
 */
[[noreturn]] void failSetup(const __FlashStringHelper *reason) {
  failSetup(reinterpret_cast<const char *>(reason));
}

/**
 * @see failSetup(const char *reason)
 */
[[noreturn]] void failSetup(const String &s) {
  failSetup(s.c_str());
}

void printConnectStatus(bool inSetup, int status) {
  const char *radioStatus = getWl_status_t(status);
  Serial.print(F("Radio status: "));
  Serial.println(radioStatus);
  clearDisplay();
  if (inSetup) {
    setDisplayCursor(0, 0);
    lcd.print(F("Running setup (2/5)"));
    setDisplayCursor(0, 1);
    lcd.print(F("Connecting to WiFi"));
  } else {
    setDisplayCursor(0, 0);
    lcd.print(F("Starting up (1/2)"));
    setDisplayCursor(0, 1);
    lcd.print(F("Connecting to WiFi"));
  }
  if (status == WL_NO_SHIELD) {
    setDisplayCursor(0, 2);
    lcd.print(F("SETUP FAILED!!!"));
    setDisplayCursor(0, 3);
    lcd.print(F("WiFi shield missing!"));
    flushDisplay();
    // don't continue:
    // TODO: Talk back to the server here if setup is happening so the user can see the issue in-app.
    // while (true);
  }
  setDisplayCursor(0, 2);
  lcd.print(F("Status:"));
  setDisplayCursor(0, 3);
  lcd.print(radioStatus);
  flushDisplay();
}

///Waits setupWaitTimeSecs. If any serial input is detected, it is read out of the buffer
///and discarded. The only purpose of the text coming over serial is for betterstat-server
///to let this device know that the server is attempting to configure it over serial.
bool checkIfSerialCommAttempted() {
  bool serialCommunicationAttempted = false;
  for (int waitTimeRemainingSecs = setupWaitTimeSecs; waitTimeRemainingSecs > -1; waitTimeRemainingSecs--) {
    printCheckingForInputSeconds(waitTimeRemainingSecs);
    delay(1000);
    if (Serial.available() > 0) {
      //break the loop early to continue to the next part
      break;
    }
  }
  if (Serial.available() > 0) {
    //There's at least one byte coming in over serial!
    serialCommunicationAttempted = true;
    clearDisplay();
    setDisplayCursor(0, 0);
    lcd.print(F("Connected to device"));
    setDisplayCursor(0, 1);
    lcd.print(F("over serial!"));
    flushDisplay();
    delay(1500);
    while (Serial.available() > 0) {
//      Serial.print("Available = ");
//      Serial.println(Serial.available());
      //Flush out whatever was sent over serial until there's nothing left
      //We don't want to read in erroneous bytes when the server starts talking to us for real
      Serial.read();
      //Are we reading too fast? It seems like Serial.available() returns 0 even when there's move to be read
      delay(50);
    }
    Serial.println(F("WAITING_FOR_SETUP"));
    clearDisplay();
    setDisplayCursor(0, 0);
    lcd.print(F("Waiting for user"));
    setDisplayCursor(0, 1);
    lcd.print(F("to start setup"));
    flushDisplay();
    while (!Serial.available()) {
      //Wait to start the setup until the user is ready
      delay(50);
    }
    while (Serial.available() > 0) {
//      Serial.print("Available = ");
//      Serial.println(Serial.available());
      //Flush out whatever was sent over serial until there's nothing left
      //We don't want to read in erroneous bytes when the server starts talking to us for real
      Serial.read();
      //Are we reading too fast? It seems like Serial.available() returns 0 even when there's move to be read
      delay(50);
    }
  }
  clearDisplay();
  return serialCommunicationAttempted;
}

void wifiCredsSetup() {
  setDisplayCursor(0, 2);
  lcd.print(F("Reading terminal.   "));
  setDisplayCursor(0, 3);
  lcd.print(F("Waiting for input..."));
  flushDisplay();

  WiFiCredsStruct userEnteredCreds = promptForCreds();
  putCreds(userEnteredCreds.ssid, userEnteredCreds.password);
  clearDisplay();
  setDisplayCursor(0, 2);
  lcd.print(F("Received credentials"));
  flushDisplay();
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

//void printWiFiData() {
//  // print your WiFi shield's IP address:
//  IPAddress ip = WiFi.localIP();
//  Serial.print("IP Address: ");
//  Serial.println(ip);
//  Serial.println(ip);
//
//  // print your MAC address:
//  byte mac[6];
//  WiFi.macAddress(mac);
//  Serial.print("MAC address: ");
//  printMacAddress(mac);
//
//}
//
//void printCurrentNet() {
//  // print the SSID of the network you're attached to:
//  Serial.print("SSID: ");
//  Serial.println(WiFi.SSID());
//
//  // print the MAC address of the router you're attached to:
//  byte bssid[6];
//  WiFi.BSSID(bssid);
//  Serial.print("BSSID: ");
//  printMacAddress(bssid);
//
//  // print the received signal strength:
//  long rssi = WiFi.RSSI();
//  Serial.print("signal strength (RSSI):");
//  Serial.println(rssi);
//
//  // print the encryption type:
//  byte encryption = WiFi.encryptionType();
//  Serial.print("Encryption Type:");
//  Serial.println(encryption, HEX);
//  Serial.println();
//}

void wifiConnect(bool inSetup) {
  int status = WL_IDLE_STATUS;     // the WiFi radio's status
  printConnectStatus(inSetup, status);
//  delay(setupStepTransitionDelay);
  while (status != WL_CONNECTED) {
    // Connect to WPA/WPA2 network:
    WiFiCredsStruct creds = getCreds();
    status = WiFi.begin(creds.ssid, creds.password);
    // wait 10 seconds for connection:
    delay(15000);
    printConnectStatus(inSetup, status);
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(ipAddressToString(WiFi.localIP()));
//  printCurrentNet();
//  printWiFiData();
}

void doServerCredsSetup() {
  setDisplayCursor(0, 2);
  lcd.print(F("Reading terminal.   "));
  setDisplayCursor(0, 3);
  lcd.print(F("Waiting for input..."));
  flushDisplay();

  ServerCredsStruct userEnteredCreds = promptForServerCreds();
  putServerCreds(userEnteredCreds.username, userEnteredCreds.password);

  clearDisplay();
  setDisplayCursor(0, 2);
  lcd.print(F("Received credentials"));
  flushDisplay();
}

void doServerHostnameOrIpSetup() {
  setDisplayCursor(0, 2);
  lcd.print(F("Reading terminal.   "));
  setDisplayCursor(0, 3);
  lcd.print(F("Waiting for input..."));
  flushDisplay();

  HostnameStruct hostname_struct = promptForHostname();
  putHostname(hostname_struct.isAnIP, hostname_struct.isSecure, hostname_struct.port, hostname_struct.hostname);

  clearDisplay();
  setDisplayCursor(0, 2);
  lcd.print(F("Received hostname"));
  flushDisplay();
}

void confirmServerCommunication() {
  HostnameStruct hostname_struct = getHostname();
  HttpClient client = getClient(hostname_struct.hostname, hostname_struct.port, hostname_struct.isSecure);
  bool connectSuccess;
  Serial.print(F("Connecting to server: "));
  Serial.println(String(hostname_struct.hostname) + ":" + hostname_struct.port + " with HTTP"
                     + (hostname_struct.isSecure ? "S" : ""));
  connectSuccess = client.connect(hostname_struct.hostname, hostname_struct.port);
  if (!connectSuccess) {
    Serial.println("ERROR CONNECTING");
    failSetup(F("Failed to connect to server"));
  } else {
    Serial.println("Connected to server!");
    client.beginRequest();
    client.get("/thermostat-api/hello-world");
    client.sendBasicAuth(getServerCreds().username, getServerCreds().password);
    client.endRequest();

    // read the status code and body of the response
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();

    if (statusCode != 200 && !response.equals("Ok"))
      failSetup(String(F("Unexpected response from server: ")) + statusCode + "\n" + response);

    client.stop();
  }
}

void finalizeSetup() {
  setSetUp(true);
  Serial.println(F("SETUP_COMPLETE"));
}

///Sets up this thermostat to be able to talk to the betterstat server.
///Delays exist in this function so that the user can follow along.
void setupThermostat() {
  //<editor-fold desc="Set up WiFi credentials">
  setDisplayCursor(0, 0);
  lcd.print(F("Running setup (1/5)"));
  setDisplayCursor(0, 1);
  lcd.print(F("Setting up WiFi"));
  flushDisplay();

  wifiCredsSetup();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Connect to WiFi">
  clearDisplay();
  setDisplayCursor(0, 0);
  lcd.print(F("Running setup (2/5)"));
  setDisplayCursor(0, 1);
  lcd.print(F("Connecting to WiFi"));
  flushDisplay();

  wifiConnect(true);
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Get auth token from server">
  clearDisplay();
  setDisplayCursor(0, 0);
  lcd.print(F("Running setup (3/5)"));
  setDisplayCursor(0, 1);
  lcd.print(F("Getting credentials"));
  flushDisplay();

  doServerCredsSetup();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Save hostname or IP to EEPROM">
  clearDisplay();
  setDisplayCursor(0, 0);
  lcd.print(F("Running setup (4/5)"));
  setDisplayCursor(0, 1);
  lcd.print(F("Saving host IP/name"));
  flushDisplay();

  doServerHostnameOrIpSetup();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Test communication with server">
  clearDisplay();
  setDisplayCursor(0, 0);
  lcd.print(F("Running setup (5/5)"));
  setDisplayCursor(0, 1);
  lcd.print(F("Talking to server"));
  flushDisplay();

  confirmServerCommunication();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Setup complete">
  clearDisplay();
  setDisplayCursor(0, 0);
  lcd.print(F("Setup complete!"));
  setDisplayCursor(0, 1);
  lcd.print(F("Please look at the"));
  setDisplayCursor(0, 2);
  lcd.print(F("app for next steps."));
  setDisplayCursor(0, 3);
  lcd.print(clearString);
  flushDisplay();

  finalizeSetup();
  while (true);
  //</editor-fold>
}

void startThermostat() {
  //<editor-fold desc="Connect to WiFi">
  setDisplayCursor(0, 0);
  lcd.print(F("Starting up (1/2)"));
  setDisplayCursor(0, 1);
  lcd.print(F("Connecting to WiFi"));
  flushDisplay();
  wifiConnect(false);
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Test communication with server">
  setDisplayCursor(0, 0);
  lcd.print(F("Starting up (2/2)"));
  setDisplayCursor(0, 1);
  lcd.print(F("Talking to server "));
  flushDisplay();
//  TODO: We don't have this set up server-side yet. Therefore, there's no Arduino implementation for it yet either.
//  confirmServerCommunication();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Startup complete">
  setDisplayCursor(0, 0);
  lcd.print(F("Startup complete!"));
  setDisplayCursor(0, 1);
  lcd.print(clearString);
  flushDisplay();
  delay(setupStepTransitionDelay);
  //</editor-fold>
}

void runSetupIfNecessary() {
  initConfigStorage();
  if (checkIfSerialCommAttempted() || !isSetUp())
    setupThermostat();
  startThermostat();
}