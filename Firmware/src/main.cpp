#include <Arduino.h>
#include "keyboard.h"
#include "menu.h"
// put function declarations here:
int myFunction(int, int);

void setup() {
  //init de MAQUINAS DE ESTADOS Keypad
  initKebpoard();
  //init de MEF de menu
  initMenu();


  
}

void loop() {
  //actualizacion de MEF de teclado
  char button = updateKeyboard();
  //actualizacion de MEF de menu
  updateMenu(button);

  //otras actividades no bloqueantes
  delay(50);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}