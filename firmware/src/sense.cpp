#include <Arduino.h>
#include "sense.h"

#define ISENSE_PIN                      1
#define VSENSE_PIN                      3 
#define TSENSE_PIN                      4
#define THERMISTOR_REFERENCE_RESISTANCE 200000.0
#define REFERENCE_VOLTAGE               1.1
#define THERMISTOR_NOMINAL              100000.0
#define THERMISTOR_TEMPERATURE_NOMINAL  25.0
#define A_COEFFICIENT                   0.9931293513e-3
#define B_COEFFICIENT                   1.775037915e-4
#define C_COEFFICIENT                   2.079274588e-7
#define BETA                            3950
#define VOLTAGE_R1                      25500.0
#define VOLTAGE_R2                      1000.0


//******************** TEMPERATURE SENSE *********************** using Steinhart-Hart equation
void readTemp(float* temperature) {
    float rawValue = analogRead(TSENSE_PIN);
    float voltage = rawValue * REFERENCE_VOLTAGE / 4095.0;                //Input voltage
    float resistance =  200000.0 / ((3.3/voltage)-1);                     //Resistance calculation    // R2=R1 / ((Vcc/Vr2)-1);
    float logR = log(resistance);
    *temperature = (1/(A_COEFFICIENT + B_COEFFICIENT * logR + C_COEFFICIENT * logR * logR * logR))-273.15;    //Temperature calculation   // 1/T = A + B * log(R) + C * [log(R)^3]

          //https://www.thinksrs.com/downloads/PDFs/ApplicationNotes/LDC%20Note%204%20NTC%20Calculatorold.pdf
          //https://www.thinksrs.com/downloads/programs/therm%20calc/ntccalibrator/ntccalculator.html
}


//******************** CURRENT SENSE ***********************
void readAmps(float* amps) {
  float Vref = 1.1;
  float amplification = 4.0;
  float resistance = 0.05;

  float Vadc = (Vref / 4095.0) * analogRead(ISENSE_PIN);     // Input voltage
  float voltage = Vadc /4;                                   // Opam amplification
  *amps = voltage / resistance;                              // Ohm law
}


//******************** VOLTS SENSE ***********************
void readvolts(float* volts) {
  float bits = analogRead(VSENSE_PIN)*1.02;
  float Vadc = (REFERENCE_VOLTAGE / 4095.0) * bits;           // Input voltage
  *volts = Vadc * (VOLTAGE_R1 + VOLTAGE_R2) / VOLTAGE_R2;     // Voltage divider
}


//******************** POWER SENSE ***********************
void readPower(float* volts, float* amps, float* watts) {
  *watts = *volts * *amps;
}


//******************** GATE SWITCH SENSE ***********************
void readSwitch(bool* gateSwitch, bool senseSw){
  *gateSwitch = senseSw;
}