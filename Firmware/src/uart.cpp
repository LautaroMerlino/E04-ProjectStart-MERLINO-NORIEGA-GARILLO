#include <Arduino.h>
#include "uart.h"
#include "SensorHumedad.h"
#include "SensorTemperatura.h"

#define BAUD_RATE 9600

void uartInit(void) {
    Serial.begin(BAUD_RATE);
    pinMode(LED_BUILTIN, OUTPUT);
}

void uartRead(void) {
    if (Serial.available() > 0) {
        char command = Serial.read();

        if (command == '\r' || command == '\n') {
            return;
        }

        if (command == 'E') {
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.println("OK");
        } else if (command == 'A') {
            digitalWrite(LED_BUILTIN, LOW);
            Serial.println("OK");
        } else if (command == 'T') {
            float temp = sensorTemperaturaLeer();
            if (temp < 0) {
                Serial.println("ERROR");
            } else {
                sensorTemperaturaEnviar(temp);
            }
        } else if (command == 'H') {
            float hum = SensorHRead();
            if (hum < 0) {
                Serial.println("ERROR");
            } else {
                sensorHumedadEnviar(hum);
            }
        } else if (command == 'S') {
            float temp = sensorTemperaturaLeer();
            float hum = SensorHRead();
            if (temp < 0 || hum < 0) {
                Serial.println("ERROR");
            } else {
                Serial.print("T:");
                Serial.print(temp, 1);
                Serial.print(";H:");
                Serial.println(hum, 1);
            }
        } else {
            Serial.println("ERROR");
        }
    }
}