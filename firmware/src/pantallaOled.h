#ifndef PANTALLAOLED_H
#define PANTALLAOLED_H

#include <Arduino.h>
#include "SSD1306Wire.h"


void imprimeixOled(String text, SSD1306Wire& display);
void imprimeixOledValors(float* amps, float* volts, float* watts, bool* gateSwitch,  SSD1306Wire& display);      //128x32, General 0.91inch OLED display



#endif