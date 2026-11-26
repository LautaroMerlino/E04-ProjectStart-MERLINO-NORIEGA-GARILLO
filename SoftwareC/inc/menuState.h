#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "serial.h"

typedef enum {
    STATE_INIT,
    STATE_ESPERA,
    STATE_LEER_SENSORES,
    STATE_ACTIVAR_LEDS,
    STATE_DATOS_USUARIO,
    STATE_ENCENDER_LED,
    STATE_APAGAR_LED,
    STATE_SALIR
} MenuState;

void menuStateInit(void);
void menuStateUpdate(char letter);

#endif