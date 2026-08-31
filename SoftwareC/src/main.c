/* @file main.c
 * @author Dev-1 (lautimerlino07@gmail.com)
 * @brief Programa principal con superloop para lectura de teclado
 * @version 0.1
 * @date 2026-05-18
 */


@copyright Copyright (c) 2026
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "../inc/menu.h"

int main() 
{    //Seccion de inicializacion oconfigurancion
    char InputChar=' ';
    menuInit();
    printf("Presione una tecla (para salir presione ESC):\n");

    while (1)
    {//seccion loop infinito
        /**@brief En la seccion loop infinito esta leyendo la entrada del caracter de la consola si hacer uso del buffer del teclado
         * por eso el uso de la libreria <conio.h> y la funcion getch (al usar getchar() no leia el ESC por el enter
         * y se mantenia en loop).
         */
        InputChar = getch();
        printf("%c\n", InputChar);
        //Si se presiona escape
        if (InputChar == 27) 
        {
            //Salir del programa
            break;
        }
        if(InputChar == '\n'){
continue;
        }
        else{
            //pasar el carácter correspondiente como parámetro a la función void menuUpdate(char);
            menuUpdate(InputChar);
        }
    }


    return 0;
}
