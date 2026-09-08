#include <Arduino.h>
#include "uart.h"

#define BAUD_RATE 9600

void uartInit(void) {
    Serial.begin(BAUD_RATE);
}

void uartRead(void) {
    if (Serial.available() > 0) {
        char command = Serial.read();
        
        if (command == 'E') {
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.println("OK");
        } else if (command == 'A') {
            digitalWrite(LED_BUILTIN, LOW);
            Serial.println("OK");
        } else if (command == 'T') {
            float temp = sensorTemperaturaLeer();
            sensorTemperaturaEnviar(temp);
        } else {
            Serial.println("ERROR");
        }
    }
}