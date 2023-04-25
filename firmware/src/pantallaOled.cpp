#include <Arduino.h>
#include "pantallaOled.h"
#include "SSD1306Wire.h"

//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
//#include <Fonts/FreeSerif9pt7b.h>

void imprimeixOled(String text, SSD1306Wire& display){      //128x32, General 0.91inch OLED display
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);//10 16 24
    display.drawString(0, 0, text);
    display.display();
/*
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    
    
    display.drawRect(0, 0, 128, 22);
    display.drawRect(43, 0, 43, 22);
    //display.drawRect(84, 0, 128, 26);
    display.setFont(ArialMT_Plain_16);//10 16 24
    display.drawString(5, 2, "1.23");   //Amps
    display.drawString(48, 2, "12.3");   //Volts
    display.drawString(91, 2 , "12.3");   //Watts
    display.setFont(ArialMT_Plain_10);//10 16 24
    display.drawString(8, 22, "Amps");
    display.drawString(53, 22, "Volts");
    display.drawString(94, 22, "Watts");
    display.drawLine(0, 32, 50, 32);
    display.display();*/
}

void imprimeixOledValors(float* amps, float* volts, float* watts, bool* gateSwitch, SSD1306Wire& display){      //128x32, General 0.91inch OLED display
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    
    
    display.drawRect(0, 0, 128, 22);
    display.drawRect(43, 0, 43, 22);
    //display.drawRect(84, 0, 128, 26);
    display.setFont(ArialMT_Plain_16);//10 16 24
    display.drawString(5, 2, String(*amps, 2));   //Amps
    if(*volts>10){
        display.drawString(48, 2, String(*volts, 1));   //Volts
    }else{
        display.drawString(48, 2, String(*volts, 2));   //Volts
    }
    if(*watts>10){
        display.drawString(91, 2 , String(*watts, 1));   //Watts
    }else{
        display.drawString(91, 2 , String(*watts, 2));   //Watts
    }
    
    display.setFont(ArialMT_Plain_10);//10 16 24
    if(*gateSwitch){
        display.drawString(8, 21, "Amps");
        display.drawString(53, 21, "Volts");
        display.drawString(94, 21, "Watts");
    }else{
        display.fillRect(0, 22, 128, 32);
        //display.drawString(15, 21, "Gate  switch  closed");
    }
    //display.drawLine(0, 32, 50, 32);
    display.display();
}

