#include <Arduino.h>
#include <Display.h>

void setup() {
    initDisplay();
    Serial.begin(9600);
}

void loop() {
    beginNewText();
    printOnLine(0,"Hi there!");
    printOnLine(1,"My name is Ryan.");
    endText();
    delay(3000);
    beginUpdateText();
    printOnLine(2,"Ummmm...");
    endText();
    delay(1500);
    beginUpdateText();
    printOnLine(3,"You got a name?");
    endText();
    while(true){
        Serial.println("IN LOOP");
        delay(5000);
    }
}