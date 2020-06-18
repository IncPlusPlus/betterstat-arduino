/*****************************************
 * name: I2C LCD2004
 * function: You should now see your I2C LCD2004 display "Hello,world!","IIC/I2C LCD2004"
 * "20 cols, 4 rows","www.sunfounder.com"
 ********************************/
//Email:service@sunfounder.com
//Website:www.sunfounder.com

/********************************/
// include the library code
#include <Arduino.h>
#include <display_variables.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Display.h>
LiquidCrystal_I2C
    lcd(0x27, DISPLAY_COLUMNS, DISPLAY_ROWS); // set the LCD address to 0x27 for a 16 chars and 2 line display

void initDisplay() {
  lcd.init();
  lcd.backlight();
}