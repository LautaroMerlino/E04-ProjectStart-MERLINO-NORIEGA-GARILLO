#include <Arduino.h>
#include "..\include\uart.h"
#include "..\inc\SensorHumedad.h"

#define BAUD_RATE 9600

void uartInit(void) {
    Serial.begin(BAUD_RATE);    
}

void uartRead(void) {
float humidityVoltage = SensorHRead();

    if (Serial.available() > 0) {
        char command = Serial.read();
        
        if (command == 'E') {
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.println("voltaje sensor humedad: " + String(humidityVoltage) + " V");
            Serial.println("Valores de PWM correspondientes: " + String((humidityVoltage * 255.0) / 3.3));
            Serial.println("OK");
        } else if (command == 'A') {
            digitalWrite(LED_BUILTIN, LOW);
            Serial.println("OK");
        } else {
            Serial.println("ERROR");
        }
    }
}