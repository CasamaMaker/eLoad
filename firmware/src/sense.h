#ifndef SENSE_H
#define SENSE_H

#include <Arduino.h>

void readTemp(float* temperature);
void readAmps(float* amps);
void readvolts(float* volts);
void readPower(float* volts, float* amps, float* watts);
void readSwitch(bool* gateSwitch, bool senseSw);

#endif