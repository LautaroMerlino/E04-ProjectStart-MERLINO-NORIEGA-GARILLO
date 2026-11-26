#include "../include/menu.h"

#include "../include/SensorHumedad.h"
#include "../include/SensorTemperatura.h"
#include <string.h>

namespace {
void menuShowMain() {
    Serial.println();
    Serial.println(F("=== Menu principal ==="));
    Serial.println(F("1. Leer sensores"));
    Serial.println(F("2. Activar/desactivar actuador"));
    Serial.println(F("3. Cargar nuevo usuario"));
    Serial.println(F("4. Consultar registro de usuarios"));
}

void menuReturnToMain(MenuContext *context) {
    context->currentState = MENU_STATE_MAIN;
    context->pendingUserIdLength = 0;
    context->pendingUserId[0] = '\0';
    menuShowMain();
}

void menuShowSensorReadings() {
    const float temperature = sensorTemperaturaLeer();
    const float humidity = SensorHRead();

    Serial.println(F("=== Lecturas de sensores ==="));
    if (temperature < 0 || humidity < 0) {
        Serial.println(F("Error al leer los sensores."));
    } else {
        Serial.print(F("Temperatura: "));
        Serial.print(temperature, 1);
        Serial.println(F(" C"));
        Serial.print(F("Humedad: "));
        Serial.print(humidity, 1);
        Serial.println(F(" %"));
    }
    Serial.println(F("Presione * o # para volver."));
}

bool menuUserExists(const MenuContext *context, const char *userId) {
    for (uint8_t i = 0; i < context->userCount; ++i) {
        if (strcmp(context->users[i], userId) == 0) {
            return true;
        }
    }
    return false;
}

void menuShowUserLog(const MenuContext *context) {
    Serial.println(F("=== Registro de usuarios ==="));
    if (context->userCount == 0) {
        Serial.println(F("No hay usuarios registrados."));
    }
    for (uint8_t i = 0; i < context->userCount; ++i) {
        Serial.print(i + 1);
        Serial.print(F(". "));
        Serial.println(context->users[i]);
    }
    Serial.println(F("Presione * o # para volver."));
}
}

void menuInit(MenuContext *context) {
    if (context == nullptr) {
        return;
    }

    context->currentState = MENU_STATE_MAIN;
    context->actuatorActive = false;
    context->userCount = 0;
    context->pendingUserIdLength = 0;
    context->pendingUserId[0] = '\0';

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    menuShowMain();
}

void menuUpdate(MenuContext *context, char key) {
    if (context == nullptr || key == '\0') {
        return;
    }

    switch (context->currentState) {
        case MENU_STATE_MAIN:
            switch (key) {
                case '1':
                    context->currentState = MENU_STATE_SENSOR_READINGS;
                    menuShowSensorReadings();
                    break;
                case '2':
                    context->currentState = MENU_STATE_ACTUATOR_CONTROL;
                    Serial.println(F("Actuador: presione 1 para alternar, * para volver."));
                    break;
                case '3':
                    context->currentState = MENU_STATE_ADD_USER;
                    context->pendingUserIdLength = 0;
                    context->pendingUserId[0] = '\0';
                    Serial.println(F("Ingrese ID (hasta 8 teclas); # guarda, * cancela."));
                    break;
                case '4':
                    context->currentState = MENU_STATE_USER_LOG;
                    menuShowUserLog(context);
                    break;
                default:
                    break;
            }
            break;

        case MENU_STATE_SENSOR_READINGS:
        case MENU_STATE_USER_LOG:
            if (key == '*' || key == '#') {
                menuReturnToMain(context);
            }
            break;

        case MENU_STATE_ACTUATOR_CONTROL:
            if (key == '1') {
                context->actuatorActive = !context->actuatorActive;
                digitalWrite(LED_BUILTIN, context->actuatorActive ? HIGH : LOW);
                Serial.println(context->actuatorActive ? F("Actuador activado.") : F("Actuador desactivado."));
            } else if (key == '*') {
                menuReturnToMain(context);
            }
            break;

        case MENU_STATE_ADD_USER:
            if (key == '*') {
                menuReturnToMain(context);
            } else if (key == '#') {
                if (context->pendingUserIdLength == 0) {
                    Serial.println(F("El ID no puede estar vacio."));
                } else if (menuUserExists(context, context->pendingUserId)) {
                    Serial.println(F("Ese ID ya esta registrado."));
                } else if (context->userCount >= MENU_MAX_USERS) {
                    Serial.println(F("El registro esta completo."));
                } else {
                    strcpy(context->users[context->userCount], context->pendingUserId);
                    ++context->userCount;
                    Serial.print(F("Usuario registrado: "));
                    Serial.println(context->pendingUserId);
                    menuReturnToMain(context);
                }
            } else if (context->pendingUserIdLength < MENU_USER_ID_LENGTH) {
                context->pendingUserId[context->pendingUserIdLength++] = key;
                context->pendingUserId[context->pendingUserIdLength] = '\0';
                Serial.print('*');
            }
            break;

        default:
            context->currentState = MENU_STATE_MAIN;
            menuShowMain();
            break;
    }
}
