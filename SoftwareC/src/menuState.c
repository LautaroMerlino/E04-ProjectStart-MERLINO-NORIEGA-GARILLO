#include <stdio.h>
#include "menuState.h"

static MenuState currentState = STATE_INIT;

void menuStateInit(void) {
    currentState = STATE_INIT;
    printf("Bienvenido al sistema\n");
    printf("1 - Leer sensores\n");
    printf("2 - Activar leds\n");
    printf("3 - Datos de usuario\n");
    printf("4 - Encender Led\n");
    printf("5 - Apagar Led\n");
    printf("ESC - Salir\n");
    currentState = STATE_ESPERA;
}

void menuStateUpdate(char letter) {
    switch (currentState) {
        case STATE_ESPERA:
            if (letter == '1') {
                currentState = STATE_LEER_SENSORES;
                printf("Leyendo sensores...\n");
                currentState = STATE_ESPERA;
            } else if (letter == '2') {
                currentState = STATE_ACTIVAR_LEDS;
                printf("Activando leds...\n");
                currentState = STATE_ESPERA;
            } else if (letter == '3') {
                currentState = STATE_DATOS_USUARIO;
                printf("Datos de usuario...\n");
            } else if (letter == '4') {
                currentState = STATE_ENCENDER_LED;
                serialSendCommand('E');
                serialReadAndPrint();
                printf("Encendiendo Led...\n");
                currentState = STATE_ESPERA;
            } else if (letter == '5') {
                currentState = STATE_APAGAR_LED;
                serialSendCommand('A');
                serialReadAndPrint();
                printf("Apagando Led...\n");
                currentState = STATE_ESPERA;
            }
            break;
        default:
            break;
    }
}