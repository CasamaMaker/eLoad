#include <Arduino.h>
#include "sense.h"


/*void imprimeixOled(String text, SSD1306Wire& display){
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);//10 16 24
    display.drawString(0, 0, text);
    display.display();
}*/

#define THERMISTOR_PIN 4
#define REFERENCE_RESISTANCE 200000.0
#define REFERENCE_VOLTAGE 1.1
#define THERMISTOR_NOMINAL 100000.0
#define TEMPERATURE_NOMINAL 25.0
#define B_COEFFICIENT 3950
#define VCC_VOLTAGE 1.1
#define BETA  3950

String readTemp(float temperature) {
    float A = 0.9931293513e-3;
    float B = 1.775037915e-4;
    float C = 2.079274588e-7;
    float rawValue = analogRead(THERMISTOR_PIN);
    float voltage = rawValue * REFERENCE_VOLTAGE / 4095.0;
    float resistance =  200000.0 / ((3.3/voltage)-1);  // R2=R1 / ((Vcc/Vr2)-1);
    float logR = log(resistance);

    temperature = 1/(A + B * logR + C * logR * logR * logR);   // 1/T = A + B * log(R) + C * [log(R)^3]
    temperature -= 273.15;
    //https://www.thinksrs.com/downloads/PDFs/ApplicationNotes/LDC%20Note%204%20NTC%20Calculatorold.pdf
    //https://www.thinksrs.com/downloads/programs/therm%20calc/ntccalibrator/ntccalculator.html
    
    /*Serial.println(voltage);
    Serial.println(resistance);
    Serial.println(temperature);
    Serial.println("---");*/
  return String(temperature);
}


String readAmps(float* amps) {
  float Vref = 1.1;
  float amplification = 4.0;
  float resistance = 0.05;

  float Vadc = (Vref / 4095.0) * analogRead(1); // Calcula el voltaje en la entrada analógica
  float voltage = Vadc /4; // Calcula el voltaje real en el divisor de voltaje
  *amps = voltage / resistance;
  float i = *amps;
  return String(i);
}

String readvolts(float* volts) {
  float Vref = 1.1; // Tensión de referencia del ADC
  float R1 = 25500.0; // Valor de la resistencia R1 del divisor de voltaje
  float R2 = 1000.0; // Valor de la resistencia R2 del divisor de voltaje
  float bits = analogRead(3)*1.02;
  float Vadc = (Vref / 4095.0) * bits; // Calcula el voltaje en la entrada analógica
  *volts = Vadc * (R1 + R2) / R2; // Calcula el voltaje real en el divisor de voltaje
  float i = *volts;
  return String(i);
}

String readPower(float* volts, float* amps) {
  return String(*volts * *amps);
}