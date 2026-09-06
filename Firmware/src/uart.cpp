#include <Arduino.h>
#include "..\inc\uart.h"

#define LED_BUILTIN

void uartInit(void) {
    Serial.begin(115200); //inicializa el puerto serie a 115200 baudios
    pinmode(LED, OUTPUT); 
    digitalWrite(Led, LOW);
}

void uartRead(void) {
    if (Serial.available() > 0) 
    {
        char command = Serial.read();
        
        if (command == 'E') {
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.println("OK");
        } else if (command == 'A') {
            digitalWrite(LED_BUILTIN, LOW);
            Serial.println("OK");
        } else {
            Serial.println("ERROR");
        }
    }
}
