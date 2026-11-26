#include <Arduino.h>
#include "../include/Keypad.h"
#include "../include/menu.h"
#include "../include/uart.h"
#include "../include/SensorHumedad.h"
#include "../include/SensorTemperatura.h"

MenuContext menuContext;

void setup() {
  uartInit();
  sensorTemperaturaInit(); // DHT11 + salida PWM de temperatura
  SensorHInit();           // salida PWM de humedad
  keypadInit();
  menuInit(&menuContext);
}

void loop() {
  uartRead();
  menuUpdate(&menuContext, keypadUpdate());

  // Cada TIEMPO_LECTURA ms se mide con el DHT11 y se actualizan las dos salidas PWM
  static unsigned long ultimaLectura = 0;
  if (millis() - ultimaLectura >= TIEMPO_LECTURA) {
    ultimaLectura = millis();
    sensorTemperaturaLeer();            // actualiza el PWM de temperatura
    float humedad = SensorHRead();
    if (humedad >= 0) {
      PWMWrite(humedad);                // actualiza el PWM de humedad
    }
  }
  //otras actividades no bloqueantes
  delay(1);
}