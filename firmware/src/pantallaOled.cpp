#include <Arduino.h>
#include "pantallaOled.h"
#include "SSD1306Wire.h"


//******************** PRINT ANY TEXT *********************** for 128x32 oled
void imprimeixOled(String text, SSD1306Wire& display){
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);//10 16 24
    display.drawString(0, 0, text);
    display.display();
}


//******************** MAIN SCREEN *********************** for 128x32 oled
void imprimeixOledValors(float* amps, float* volts, float* watts, bool* gateSwitch, SSD1306Wire& display){
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);

    display.drawRect(0, 0, 128, 22);
    display.drawRect(43, 0, 43, 22);
    display.setFont(ArialMT_Plain_16);
    display.drawString(5, 2, String(*amps, 2));         //Amps
    if(*volts>10){
        display.drawString(48, 2, String(*volts, 1));   //Volts with one decimal
    }else{
        display.drawString(48, 2, String(*volts, 2));   //Volts with two decimal
    }
    if(*watts>10){
        display.drawString(91, 2 , String(*watts, 1));   //Watts with one decimal
    }else{
        display.drawString(91, 2 , String(*watts, 2));   //Watts  with one decimal
    }
    
    display.setFont(ArialMT_Plain_10);//10 16 24
    if(*gateSwitch){                                     //Text of mesurement         
        display.drawString(8, 21, "Amps");
        display.drawString(53, 21, "Volts");
        display.drawString(94, 21, "Watts");
    }else{
        display.fillRect(0, 22, 128, 32);
        //display.drawString(15, 21, "Gate  switch  closed");
    }
    display.display();
}

