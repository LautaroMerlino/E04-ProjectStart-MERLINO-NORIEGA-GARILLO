#include <stdio.h>
#include "Sensores.h"

void sensoresInit(void) {
    printf("Sensores inicializados.\n");
}

void sensoresRequestValue(void) {
    serialSendCommand('S');
    serialReadAndPrint();
}