#include <stdio.h>
#include "SensorTemperatura.h"

void sensorTemperaturaInit(void) {
    printf("Sensor de temperatura inicializado\n");
}

void sensorTemperaturaRequestValue(void) {
    serialSendCommand('T');
    serialReadAndPrint();
}