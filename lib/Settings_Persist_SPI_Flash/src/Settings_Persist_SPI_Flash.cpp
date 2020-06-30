#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_SPIFlash.h>
#include <Settings_Persist_SPI_Flash.hpp>

// On-board external flash (QSPI or SPI) macros should already
// defined in your board variant if supported
// - EXTERNAL_FLASH_USE_QSPI
// - EXTERNAL_FLASH_USE_CS/EXTERNAL_FLASH_USE_SPI
#if defined(EXTERNAL_FLASH_USE_QSPI)
Adafruit_FlashTransport_QSPI flashTransport;

#elif defined(EXTERNAL_FLASH_USE_SPI)
Adafruit_FlashTransport_SPI flashTransport(EXTERNAL_FLASH_USE_CS, EXTERNAL_FLASH_USE_SPI);

#else
#error No QSPI/SPI flash are defined on your board variant.h !
#endif

Adafruit_SPIFlash flash(&flashTransport);

// file system object from SdFat
FatFileSystem fatfs;

bool initConfigStorage() {
  //Shamelessly stolen from the Adafruit SPI Flash FatFs Full Usage Example

  // Initialize flash library and check its chip ID.
  if (!flash.begin()) {
    Serial.println("Error, failed to initialize flash chip!");
    return false;
  }
  Serial.print("Flash chip JEDEC ID: 0x");
  Serial.println(flash.getJEDECID(), HEX);

  // First call begin to mount the filesystem.  Check that it returns true
  // to make sure the filesystem was mounted.
  if (!fatfs.begin(&flash)) {
    Serial.println(F("Error, failed to mount newly formatted filesystem!"));
    Serial.println(F("Was the flash chip formatted with the SdFat_format example?"));
    return false;
  }
  Serial.println("Mounted filesystem!");
  if (!fatfs.exists("/config")) {
    Serial.println("config directory not found, creating...");

    // Use mkdir to create directory (note you should _not_ have a trailing slash).
    fatfs.mkdir("/config");

    if (!fatfs.exists("/config")) {
      Serial.println("Error, failed to create directory!");
      return false;
    } else {
      Serial.println("Created directory!");
    }
  }
  File configDir = fatfs.open("/config");
  Serial.println("Listing children of directory /config:");
  File child = configDir.openNextFile();
  while (child) {
    char filename[64];
    child.getName(filename, sizeof(filename));

    // Print the file name and mention if it's a directory.
    Serial.print("- ");
    Serial.print(filename);
    if (child.isDirectory()) {
      Serial.print(" (directory)");
    }
    Serial.println();
    // Keep calling openNextFile to get a new file.
    // When you're done enumerating files an unopened one will
    // be returned (i.e. testing it for true/false like at the
    // top of this while loop will fail).
    child = configDir.openNextFile();
  }
  return true;
}

void rmFile(const char *path) {
  bool fileExists = fatfs.exists(path);
  if (fileExists) {
    fatfs.remove(path);
  } else {
    Serial.print("Didn't delete '");
    Serial.print(path);
    Serial.println("' as it wasn't there. No big deal.");
  }
}

void rmDir(const char *path) {
  File configDir = fatfs.open(path);
  if (!configDir) {
    Serial.print("Error, failed to open '");
    Serial.print(path);
    Serial.println("' directory!");
    return;
  }

  Serial.println("Deleting /config directory and everything inside it...");
  if (!configDir.rmRfStar()) {
    Serial.print("Error, couldn't delete '");
    Serial.print(path);
    Serial.println("' directory!");
    return;
  }
  // Check that the directory is really deleted.
  if (fatfs.exists(path)) {
    Serial.print("Error, directory '");
    Serial.print(path);
    Serial.println("' was not deleted!");
    return;
  }
  Serial.print("'");
  Serial.print(path);
  Serial.println("' directory was deleted!");
}

bool isSetUp() {
  File readFile = fatfs.open("/config/configured.txt", FILE_READ);
  if (!readFile) {
    Serial.println(F("Error, failed to open /config/configured.txt for reading!"));
    Serial.println(F("Setup most likely still needs to be run."));
    return false;
  }
  String line = readFile.readStringUntil('\r');
  line.trim();
  readFile.close();
  return line.equals(F("configured"));
}

void setSetUp(bool isSetUp) {
  clearSetUp();
  if (isSetUp) {
    //create /config/configured.txt with contents "configured\n"
    File writeFile = fatfs.open("/config/configured.txt", FILE_WRITE);
    if (!writeFile) {
      Serial.println(F("Error, failed to open configured.txt for writing!"));
      return;
    }
    Serial.println(F("Opened file /config/configured.txt for writing/appending..."));
    writeFile.println(F("configured"));
    // Close the file when finished writing.
    writeFile.close();
    Serial.println(F("Wrote to file /config/configured.txt!"));
  }
  //Otherwise, just leave the file deleted
}

void clearSetUp() {
  rmFile("/config/configured.txt");
}

void putCreds(char *ssid, char *password) {
  rmFile("/config/wificreds.txt");
  //create /config/wificreds.txt with the SSID and password on their own two lines followed by a newline
  File writeFile = fatfs.open("/config/wificreds.txt", FILE_WRITE);
  if (!writeFile) {
    Serial.println(F("Error, failed to open wificreds.txt for writing!"));
    return;
  }
//  Serial.println(F("Opened file /config/wificreds.txt for writing/appending..."));
  writeFile.println(ssid);
  writeFile.println(password);
  // Close the file when finished writing.
  writeFile.close();
//  Serial.println(F("Wrote to file /config/wificreds.txt!"));
}

struct WiFiCredsStruct getCreds() {
  WiFiCredsStruct creds{};
  File readFile = fatfs.open("/config/wificreds.txt", FILE_READ);
  if (!readFile) {
    Serial.println(F("Error, failed to open /config/wificreds.txt for reading!"));
    return creds;
  }
  String ssid = readFile.readStringUntil('\r');
  ssid.trim();
  String password = readFile.readStringUntil('\r');
  password.trim();
  Serial.println("'" + String(ssid) + "'");
  Serial.println("'" + String(password) + "'");
  readFile.close();
  ssid.toCharArray(creds.ssid, sizeof(creds.ssid));
  password.toCharArray(creds.password, sizeof(creds.password));
  return creds;
}

void clearCreds() {
  rmFile("/config/wificreds.txt");
}

bool credsExist() {
  return fatfs.exists("/config/wificreds.txt");
}

void putServerCreds(char *username, char *password) {
  rmFile("/config/servercreds.txt");
  //create /config/servercreds.txt with the SSID and password on their own two lines followed by a newline
  File writeFile = fatfs.open("/config/servercreds.txt", FILE_WRITE);
  if (!writeFile) {
    Serial.println(F("Error, failed to open servercreds.txt for writing!"));
    return;
  }
//  Serial.println(F("Opened file /config/servercreds.txt for writing/appending..."));
  writeFile.println(username);
  writeFile.println(password);
  // Close the file when finished writing.
  writeFile.close();
//  Serial.println(F("Wrote to file /config/servercreds.txt!"));
}

struct ServerCredsStruct getServerCreds() {
  ServerCredsStruct creds{};
  File readFile = fatfs.open("/config/servercreds.txt", FILE_READ);
  if (!readFile) {
    Serial.println(F("Error, failed to open /config/servercreds.txt for reading!"));
    return creds;
  }
  String username = readFile.readStringUntil('\r');
  username.trim();
  String password = readFile.readStringUntil('\r');
  password.trim();
  readFile.close();
  username.toCharArray(creds.username, sizeof(creds.username));
  password.toCharArray(creds.password, sizeof(creds.password));
  return creds;
}

void clearServerCreds() {
  rmFile("/config/servercreds.txt");
}

bool serverCredsExist() {
  return fatfs.exists("/config/servercreds.txt");
}

void putHostname(bool hostnameIsAnIP, bool isSecure, uint16_t port, char *hostname) {
  rmFile("/config/hostname.txt");
  //create /config/hostname.txt with the SSID and password on their own two lines followed by a newline
  File writeFile = fatfs.open("/config/hostname.txt", FILE_WRITE);
  if (!writeFile) {
    Serial.println(F("Error, failed to open hostname.txt for writing!"));
    return;
  }
//  Serial.println(F("Opened file /config/hostname.txt for writing/appending..."));
  writeFile.println(hostnameIsAnIP);
  writeFile.println(isSecure);
  writeFile.println(port);
  writeFile.println(hostname);
  // Close the file when finished writing.
  writeFile.close();
//  Serial.println(F("Wrote to file /config/hostname.txt!"));
}

struct HostnameStruct getHostname() {
  HostnameStruct hostname_struct{};
  File readFile = fatfs.open("/config/hostname.txt", FILE_READ);
  if (!readFile) {
    Serial.println(F("Error, failed to open /config/hostname.txt for reading!"));
    return hostname_struct;
  }
  String isHostnameAnIP = readFile.readStringUntil('\r');
  isHostnameAnIP.trim();
  String isSecure = readFile.readStringUntil('\r');
  isSecure.trim();
  String port = readFile.readStringUntil('\r');
  port.trim();
  String hostnameString = readFile.readStringUntil('\r');
  hostnameString.trim();
  readFile.close();
  //TODO: cleanup prints and code duplication
  if (isHostnameAnIP.equals("1")) {
    hostname_struct.isAnIP = true;
  } else { hostname_struct.isAnIP = false; }
  if (isSecure.equals("1")) {
    hostname_struct.isSecure = true;
  } else { hostname_struct.isSecure = false; }
  hostname_struct.port = port.toInt();
  hostnameString.toCharArray(hostname_struct.hostname, sizeof(hostname_struct.hostname));
  return hostname_struct;
}

void clearHostname() {
  rmFile("/config/hostname.txt");
}

bool hostnameExists() {
  return fatfs.exists("/config/hostname.txt");
}

void clearAllSettings() {
  clearSetUp();
  clearCreds();
  clearServerCreds();
  clearHostname();
}