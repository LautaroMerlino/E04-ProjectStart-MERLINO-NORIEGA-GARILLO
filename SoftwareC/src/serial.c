#include <stdio.h>
#include "serial.h"

#define PORT_NUMBER 0    // COM1
#define BAUD_RATE   9600

void serialInit(void) {
    if (RS232_OpenComport(PORT_NUMBER, BAUD_RATE, "8N1", 0)) {
        printf("Error al abrir el puerto serie\n");
    } else {
        printf("Puerto serie inicializado\n");
    }
}

void serialSendCommand(char command) {
    unsigned char buf[1];
    buf[0] = command;
    RS232_SendBuf(PORT_NUMBER, buf, 1);
    printf("Comando enviado: %c\n", command);
}

void serialReadAndPrint(void) {
    unsigned char buf[64];
    int n = RS232_PollComport(PORT_NUMBER, buf, 63);
    if (n > 0) {
        buf[n] = '\0';
        printf("Respuesta: %s\n", (char*)buf);
    }
}