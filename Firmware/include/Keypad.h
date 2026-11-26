#ifndef KEYPAD_H
#define KEYPAD_H

#include <Arduino.h>

#define KEYPAD_DEBOUNCE_MS 40U

typedef enum {
    KEYPAD_STATE_IDLE,
    KEYPAD_STATE_DEBOUNCE_PRESS,
    KEYPAD_STATE_PRESSED,
    KEYPAD_STATE_WAIT_RELEASE,
    KEYPAD_STATE_DEBOUNCE_RELEASE
} KeypadState;

void keypadInit(void);
char keypadUpdate(void);
KeypadState keypadGetState(void);

#endif /* KEYPAD_H */