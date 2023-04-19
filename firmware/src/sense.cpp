#include <Arduino.h>
#include "sense.h"


/*void imprimeixOled(String text, SSD1306Wire& display){
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);//10 16 24
    display.drawString(0, 0, text);
    display.display();
}*/

#define REFERENCE_RESISTANCE 200000
#define REFERENCE_VOLTAGE 1.0
#define THERMISTOR_NOMINAL 100000
#define TEMPERATURE_NOMINAL 25.0
#define B_COEFFICIENT 3950

String readTemp(int THERMISTOR_PIN) {
  float sumaDeu;
  for(int i=10;i>0;i--){    
    int rawValue = analogRead(THERMISTOR_PIN);
    float voltage = rawValue * REFERENCE_VOLTAGE / 4095.0;
    float resistance = REFERENCE_RESISTANCE * voltage / (REFERENCE_VOLTAGE - voltage);

    float steinhart;
    steinhart = resistance / THERMISTOR_NOMINAL;      // (R/Ro)
    steinhart = log(steinhart);                       // ln(R/Ro)
    steinhart /= B_COEFFICIENT;                       // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURE_NOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart - 273.15;             // Invertir y convertir a Celsius
    sumaDeu += steinhart;
  }

  float final = sumaDeu/10;

  return String(final);
}


//float amps;
//float volts;



