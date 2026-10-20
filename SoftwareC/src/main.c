/* @file main.c
 * @author Dev-1 (lautimerlino07@gmail.com)
 * @brief Punto de entrada y superloop de la aplicación de consola
 * @version 0.1
 * @date 2026-05-18
 */

#include <stdio.h>
#include <conio.h>
#include "../inc/SensorTemperatura.h"
#include "../inc/Sensores.h"
#include "../inc/menuState.h"
#include "../inc/serial.h"

int main(void)
{
    int inputChar;

    serialInit();
    sensoresInit();
    sensorTemperaturaInit();
    menuStateInit();

    printf("Presione una tecla (para salir presione ESC):\n");

    while (1) {
        inputChar = getch();
        if (inputChar == 27) {
            break;
        }

        printf("%c\n", inputChar);
        menuStateUpdate((char)inputChar);
    }

    return 0;
}
