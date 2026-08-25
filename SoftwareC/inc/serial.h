#ifndef SERIAL_H
#define SERIAL_H

#include "rs232.h"

void serialInit(void);
void serialSendCommand(char command);
void serialReadAndPrint(void);

#endif