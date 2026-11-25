#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include <Arduino.h>

void initTemperatureSensor(int pin);
void initMotor(int pin);
float readTemperature();

#endif