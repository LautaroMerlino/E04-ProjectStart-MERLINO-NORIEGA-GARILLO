#include "Keypad.h"

namespace {
const uint8_t rowPins[] = {4, 5, 6, 7};
const uint8_t columnPins[] = {8, 9, 10, 11};
const char keyMap[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

KeypadState currentState = KEYPAD_STATE_IDLE;
char candidateKey = '\0';
unsigned long stateStartedAt = 0;

char scanMatrix() {
    char detectedKey = '\0';
    uint8_t detectedKeys = 0;

    for (uint8_t column = 0; column < 4; ++column) {
        for (uint8_t i = 0; i < 4; ++i) {
            pinMode(columnPins[i], INPUT);
        }
        digitalWrite(columnPins[column], LOW);
        pinMode(columnPins[column], OUTPUT);
        delayMicroseconds(3);

        for (uint8_t row = 0; row < 4; ++row) {
            if (digitalRead(rowPins[row]) == LOW) {
                detectedKey = keyMap[row][column];
                ++detectedKeys;
            }
        }
    }

    for (uint8_t i = 0; i < 4; ++i) {
        pinMode(columnPins[i], INPUT);
    }

    return detectedKeys == 1 ? detectedKey : '\0';
}
}

void keypadInit(void) {
    for (uint8_t row = 0; row < 4; ++row) {
        pinMode(rowPins[row], INPUT_PULLUP);
    }
    for (uint8_t column = 0; column < 4; ++column) {
        digitalWrite(columnPins[column], LOW);
        pinMode(columnPins[column], INPUT);
    }

    currentState = KEYPAD_STATE_IDLE;
    candidateKey = '\0';
    stateStartedAt = millis();
}

char keypadUpdate(void) {
    const unsigned long now = millis();

    switch (currentState) {
        case KEYPAD_STATE_IDLE: {
            const char key = scanMatrix();
            if (key != '\0') {
                candidateKey = key;
                stateStartedAt = now;
                currentState = KEYPAD_STATE_DEBOUNCE_PRESS;
            }
            break;
        }
        case KEYPAD_STATE_DEBOUNCE_PRESS:
            if (scanMatrix() != candidateKey) {
                candidateKey = '\0';
                currentState = KEYPAD_STATE_IDLE;
            } else if (now - stateStartedAt >= KEYPAD_DEBOUNCE_MS) {
                currentState = KEYPAD_STATE_PRESSED;
                return candidateKey;
            }
            break;
        case KEYPAD_STATE_PRESSED:
            currentState = KEYPAD_STATE_WAIT_RELEASE;
            break;
        case KEYPAD_STATE_WAIT_RELEASE:
            if (scanMatrix() == '\0') {
                stateStartedAt = now;
                currentState = KEYPAD_STATE_DEBOUNCE_RELEASE;
            }
            break;
        case KEYPAD_STATE_DEBOUNCE_RELEASE:
            if (scanMatrix() != '\0') {
                currentState = KEYPAD_STATE_WAIT_RELEASE;
            } else if (now - stateStartedAt >= KEYPAD_DEBOUNCE_MS) {
                candidateKey = '\0';
                currentState = KEYPAD_STATE_IDLE;
            }
            break;
        default:
            currentState = KEYPAD_STATE_IDLE;
            candidateKey = '\0';
            break;
    }

    return '\0';
}

KeypadState keypadGetState(void) {
    return currentState;
}