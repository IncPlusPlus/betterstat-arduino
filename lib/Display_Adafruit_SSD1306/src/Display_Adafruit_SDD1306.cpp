#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define TEXT_SIZE 1

Adafruit_SSD1306
    lcd(128, 32, &Wire); // set the LCD address to 0x27 for a 20 chars and 4 line display

void initDisplay() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  // Show image buffer on the display hardware.
  // Since the buffer is initialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  lcd.display();
  delay(1000);

  // Clear the buffer.
  lcd.clearDisplay();
  lcd.display();

  lcd.setTextSize(TEXT_SIZE);
  lcd.setTextColor(SSD1306_WHITE);
}

void clearDisplay() {
  lcd.clearDisplay();
}

void setDisplayCursor(uint8_t col, uint8_t row) {
  //With font size 1, characters are 6 px wide by 8 px tall.
  lcd.setCursor(col * 6 * TEXT_SIZE, row * 8 * TEXT_SIZE);
}

void flushDisplay() {
  lcd.display();
}