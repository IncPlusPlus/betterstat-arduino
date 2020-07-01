#pragma once

#if (DISPLAY_TYPE == 1)
#include <Display_LiquidCrystalI2c.hpp>
#elif (DISPLAY_TYPE == 2)
#include <Display_Adafruit_SDD1306.hpp>
#else
#error "Please define DISPLAY_TYPE with a valid value"
#endif