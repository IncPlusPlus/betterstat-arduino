#include <Arduino.h>
#include <IPAddress.h>

String readLine() {
  String inData = "";
  bool readingComplete = false;
  while (!readingComplete) {
    while (Serial.available() > 0) {
      char received = Serial.read();
      if (received == '\n') {
        readingComplete = true;
      } else {
        // We never want our data to include newlines or carriage returns
        inData += received;
      }
    }
  }
  return inData;
}

//String getDelimitedString(const String &s) {
//  int startIndex = s.indexOf('^');
//  int endIndex = s.indexOf('$');
//  if (startIndex == -1 || endIndex == -1)
//    return "";
//  return s.substring(startIndex + 1, endIndex);
//}

String ipAddressToString(IPAddress &ip) {
  String s="";
  for (int i=0; i<4; i++) {
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  }
  return s;
}

String ipAddressToString(uint32_t ip) {
  IPAddress ip_addr = ip;
  return ipAddressToString(ip_addr);
}
