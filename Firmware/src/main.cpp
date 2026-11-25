#include <Arduino.h>
#include "..\include\keypad.h"
#include "..\include\menu.h"
#include "..\include\uart.h"
#include "..\include\SensorHumedad.h"
#include "..\include\SensorTemperatura.h"



// put function declarations here:
int myFunction(int, int);

void setup() {
  //init de MAQUINAS DE ESTADOS Keypad
  //initKeyboard();
  //init de MEF de menu
  //initMenu();

  uartInit();
  sensorTemperaturaInit(); // DHT11 + salida PWM de temperatura
  SensorHInit();           // salida PWM de humedad
}

void loop() {
  //actualizacion de MEF de teclado
  //char button = updateKeyboard();
  //actualizacion de MEF de menu
  //updateMenu(button);

  uartRead();

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

int myFunction(int x, int y) {
  return x + y;
}