#include <EEPROM_Persist.h>
#include <Display.h>

const int setupStepTransitionDelay = 2000;

void wifiCredsSetup() {
  beginUpdateText();
  printOnLine(2, "Reading EEPROM...");
  endText();
  delay(1000);
  //true if already in EEPROM, false if prompted for and then entered
  bool credsFound = credsExist();
  if (!credsFound) {
    beginUpdateText();
    printOnLine(2, "Credentials not");
    printOnLine(3, "found in EEPROM");
    endText();
    delay(1500);
    beginUpdateText();
    printOnLine(2, "Reading terminal.");
    printOnLine(3, "Waiting for input...");
    endText();
    WiFiCreds userEnteredCreds = promptForCreds();
    putCreds(userEnteredCreds.ssid, userEnteredCreds.password);
  }
  beginUpdateText();
  printOnLine(2, "Creds provided by ");
  printOnLine(3, credsFound ? "EEPROM" : "user");
  endText();
}

void wifiConnect() {
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    beginUpdateText();
    printOnLine(2, "SETUP FAILED!!!");
    printOnLine(3, "WiFi shield missing!");
    endText();
    // don't continue:
    // TODO: Talk back to the server here if setup is happening so the user can see the issue in-app.
    while (true);
  }

  int status = WL_IDLE_STATUS;     // the WiFi radio's status
  beginUpdateText();
  printOnLine(2, "Status:");
  endText();
  printRaw(3, status);
  delay(1000);
  while (status != WL_CONNECTED) {
    // Connect to WPA/WPA2 network:
    WiFiCreds creds = getCreds();
    status = WiFi.begin(creds.ssid, creds.password);
    printRaw(3, status);
    // wait 10 seconds for connection:
    delay(10000);
    printRaw(3, WiFi.status());
  }
}

void finalizeSetup() {
  // TODO: Send message to the server to have the app prompt the user to do the next setup steps.
  setSetUp(true);
}

///Sets up this thermostat to be able to talk to the betterstat server.
///Delays exist in this function so that the user can follow along.
void setupThermostat() {
  //<editor-fold desc="Set up WiFi credentials">
  beginNewText();
  printOnLine(0, "Running setup (1/5)");
  printOnLine(1, "Setting up: WiFi");
  endText();
  wifiCredsSetup();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Connect to WiFi">
  beginNewText();
  printOnLine(0, "Running setup (2/5)");
  printOnLine(1, "Connecting to WiFi");
  endText();
  wifiConnect();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Get auth token from server">
  beginNewText();
  printOnLine(0, "Running setup (3/5)");
  printOnLine(1, "Getting auth token");
  endText();
//  TODO: We don't have this set up server-side yet. Therefore, there's no Arduino implementation for it yet either.
//  doTokenSetup();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Save hostname or IP to EEPROM">
  beginNewText();
  printOnLine(0, "Running setup (4/5)");
  printOnLine(1, "Saving host IP/name");
  endText();
//  TODO: We don't have this set up server-side yet. Therefore, there's no Arduino implementation for it yet either.
//  doServerHostnameOrIpSetup();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Test communication with server">
  beginNewText();
  printOnLine(0, "Running setup (5/5)");
  printOnLine(1, "Talking to server");
  endText();
//  TODO: We don't have this set up server-side yet. Therefore, there's no Arduino implementation for it yet either.
//  confirmServerCommunication();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Setup complete">
  beginNewText();
  printOnLine(0, "Setup complete!");
  printOnLine(1, "Please look at the");
  printOnLine(2, "app for next steps.");
  endText();
  finalizeSetup();
  while (true);
  //</editor-fold>
}

void startThermostat() {
  //<editor-fold desc="Connect to WiFi">
  beginNewText();
  printOnLine(0, "Starting up (1/2)");
  printOnLine(1, "Connecting to WiFi");
  endText();
  wifiConnect();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Test communication with server">
  beginNewText();
  printOnLine(0, "Starting up (2/2)");
  printOnLine(1, "Talking to server");
  endText();
//  TODO: We don't have this set up server-side yet. Therefore, there's no Arduino implementation for it yet either.
//  confirmServerCommunication();
  delay(setupStepTransitionDelay);
  //</editor-fold>

  //<editor-fold desc="Startup complete">
  beginNewText();
  printOnLine(0, "Startup complete!");
  endText();
//  TODO: We don't have this set up server-side yet. Therefore, there's no Arduino implementation for it yet either.
//  confirmServerCommunication();
  delay(setupStepTransitionDelay);
  //</editor-fold>
}

void runSetupIfNecessary() {
  if (!isSetUp())
    setupThermostat();
  startThermostat();
}