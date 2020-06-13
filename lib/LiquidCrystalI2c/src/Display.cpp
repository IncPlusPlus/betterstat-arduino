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
LiquidCrystal_I2C lcd(0x27, DISPLAY_COLUMNS, DISPLAY_ROWS); // set the LCD address to 0x27 for a 16 chars and 2 line display
// +1 is to have space for the null
char text[DISPLAY_ROWS][DISPLAY_COLUMNS + 1];
bool updatedRows[DISPLAY_ROWS];
int leftPaddingNecessary[DISPLAY_ROWS];
bool textBeingUpdated = false;

void outputTextToDisplay()
{
    for (int i = 0; i < DISPLAY_ROWS; i++)
    {
        if (textBeingUpdated)
        {
            if (updatedRows[i])
            {
                lcd.setCursor(0, i);
                lcd.print(text[i]);
            }
            else
            {
                continue;
            }
        }
        else
        {
            lcd.setCursor(0, i);
            lcd.print(text[i]);
        }
    }
}

void clearLeftPaddingNecessary()
{
    for (int i = 0; i < DISPLAY_ROWS; i++)
        leftPaddingNecessary[i] = 0;
}

void clearText()
{
    for (int i = 0; i < DISPLAY_ROWS; i++)
        for (int j = 0; j < DISPLAY_COLUMNS; j++)
            text[i][j] = ' ';
}

void clearUpdatedRows()
{
    for (int i = 0; i < DISPLAY_ROWS; i++)
        updatedRows[i] = false;
}

void initDisplay()
{
    clearText();
    clearUpdatedRows();
    clearLeftPaddingNecessary();
    lcd.init();
    lcd.backlight();
}

void beginNewText()
{
    clearText();
    clearLeftPaddingNecessary();
    lcd.clear();
    lcd.home();
}

void beginUpdateText()
{
    textBeingUpdated = true;
}

void endText()
{
    outputTextToDisplay();
    if (textBeingUpdated)
    {
        textBeingUpdated = false;
        clearUpdatedRows();
    }
}

void printOnLine(int line, const char *s)
{
    int leftPadding = (DISPLAY_COLUMNS - (strlen(s))) / 2;
    // In case this is too long and the value becomes negative, just don't use padding
    // so that the string does its best to fit on the line as much as possible.
    if (leftPadding < 0)
        leftPadding = 0;
    // Update the padding value to be used for this line
    leftPaddingNecessary[line] = leftPadding;

    for (int i = 0; i < leftPadding + strlen(s) && i < DISPLAY_COLUMNS; i++)
    {
        if (i < leftPadding)
        {
            text[line][i] = ' ';
        }
        else
        {
            text[line][i] = s[i - leftPadding];
        }
    }
    if (textBeingUpdated)
    {
        updatedRows[line] = true;
    }
}

const char* concat(const char *s1, const char *s2) {
  char* retVal = new char[strlen(s1)+strlen(s2)+1];
  *retVal = '\0';
  strcat(retVal,s1);
  strcat(retVal,s2);
  return retVal;
}