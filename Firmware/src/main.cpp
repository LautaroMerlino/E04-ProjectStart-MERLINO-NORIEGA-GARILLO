#include <Arduino.h>
#include "..\include\keypad.h"
#include "..\include\menu.h"
#include "..\include\uart.h"
#include "..\include\SensorHumedad.h"



// put function declarations here:
int myFunction(int, int);

void setup() {
  //init de MAQUINAS DE ESTADOS Keypad
  initKeyboard();
  //init de MEF de menu
  initMenu();

  uartInit();
}

void loop() {
  //actualizacion de MEF de teclado
  char button = updateKeyboard();
  //actualizacion de MEF de menu
  updateMenu(button);

  uartRead();

  float humidityVoltage = SensorHRead();
  Serial.println(humidityVoltage);
  PWMWrite(humidityVoltage);
  //otras actividades no bloqueantes
  delay(1);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}