/*****************************************
 * name: I2C LCD2004
 * function: You should now see your I2C LCD2004 display "Hello,world!","IIC/I2C LCD2004"
 * "20 cols, 4 rows","www.sunfounder.com"
 ********************************/
//Email:service@sunfounder.com
//Website:www.sunfounder.com

/********************************/
// include the library code
#include <LiquidCrystal_I2C.h>
#include <Display_LiquidCrystalI2c.hpp>
LiquidCrystal_I2C
    lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 20 chars and 4 line display

void initDisplay() {
  lcd.init();
  lcd.backlight();
}

void clearDisplay() {
  lcd.clear();
}

void setDisplayCursor(uint8_t col, uint8_t row) {
  lcd.setCursor(col, row);
}

void flushDisplay() {
  //Does nothing
}