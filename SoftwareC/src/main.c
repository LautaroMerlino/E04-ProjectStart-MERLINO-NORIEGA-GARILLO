/* @file main.c
 * @author Dev-1 (lautimerlino07@gmail.com)
 * @brief Programa principal con superloop para lectura de teclado
 * @version 0.1
 * @date 2026-05-18
 */


@copyright Copyright (c) 2026
#include <stdio.h>
#include <stdlib.h>
#include menu.h
#include "menuState.h"

int main() 
{
    char letter = ' ';
    //Seccion de inicializacion
    menuUpdate();
    menuStateInit();

    //Loop infinito
    while (1)
    {
        //Leer entrada de teclado
        letter = getchar();

        //Si se presiona escape
        if (letter == 27) 
        {
            //Salir del programa
            break;
        }
        


        //Si se presiona otra tecla
        //pasar el carácter correspondiente como parámetro a la función void menuUpdate(char);
        menuUpdate(letter);
        menuStateUpdate(letter);
    }


    return 0;
}
