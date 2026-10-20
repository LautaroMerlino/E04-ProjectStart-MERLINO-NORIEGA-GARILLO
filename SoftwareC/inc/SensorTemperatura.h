#ifndef SENSOR_TEMPERATURA_H
#define SENSOR_TEMPERATURA_H

#include "serial.h"

#define PIN_SENSOR_TEMP 2
#define TIEMPO_LECTURA 1000 // ms

void sensorTemperaturaInit(void);
void sensorTemperaturaRequestValue(void);

#endif
