#include <Arduino.h>
#include "pantallaOled.h"
#include "SSD1306Wire.h"

void imprimeixOled(String text, SSD1306Wire& display){
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);//10 16 24
    display.drawString(0, 0, text);
    display.display();
}