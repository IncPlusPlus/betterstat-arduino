#include <Arduino.h>
#include <Display.h>
#include <EEPROM_Persist.h>

void setup() {
  initDisplay();
//  Uncomment this line, upload the program, comment it, and reupload to start with cleared credentials.
//clearCreds();
  Serial.begin(9600);
}

void loop() {
  beginNewText();
  printOnLine(0, "Getting creds");
  printOnLine(1, "from EEPROM...");
  endText();
  delay(1000);
  //true if already in EEPROM, false if prompted for and then entered
  bool credsFound = credsExist();
  if (!credsFound) {
    beginUpdateText();
    printOnLine(1, "from terminal.");
    printOnLine(2, "Waiting for input...");
    endText();
    WiFiCreds userEnteredCreds = promptForCreds();
    putCreds(userEnteredCreds.ssid, userEnteredCreds.password);
  }
  WiFiCreds creds = getCreds();
  beginNewText();
  printOnLine(0, concat(concat("Provided by ", credsFound ? "EEPROM" : "User"), ":"));
  char *ssid = new char[19];
  *ssid = '\0';
  strcat(ssid, "SSID: ");
  strcat(ssid, creds.ssid);
  printOnLine(1, ssid);

  char *pass = new char[19];
  *pass = '\0';
  strcat(pass, "Pass: ");
  strcat(pass, creds.password);
  printOnLine(2, pass);
  endText();
}