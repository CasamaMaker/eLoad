#ifndef SENSE_H
#define SENSE_H

#include <Arduino.h>

String readTemp(float temperature);
String readAmps(float* amps);
String readvolts(float* volts);
String readPower(float* volts, float* amps);

#endif