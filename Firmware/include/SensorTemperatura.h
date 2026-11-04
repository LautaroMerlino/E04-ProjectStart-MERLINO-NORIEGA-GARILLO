#ifndef SENSOR_TEMPERATURA_H
#define SENSOR_TEMPERATURA_H

#include <Arduino.h>
#include <DHT.h>

#define PIN_SENSOR_TEMP 2
#define DHT_TYPE DHT11
#define TIEMPO_LECTURA 1000 //ms

void sensorTemperaturaInit(void);
float sensorTemperaturaLeer(void);
void sensorTemperaturaEnviar(float temperatura);

#endif