#ifndef MENU_H
#define MENU_H

#include <Arduino.h>

enum {
    MENU_MAX_USERS = 10,
    MENU_USER_ID_LENGTH = 8
};

typedef enum {
    MENU_STATE_MAIN,
    MENU_STATE_SENSOR_READINGS,
    MENU_STATE_ACTUATOR_CONTROL,
    MENU_STATE_ADD_USER,
    MENU_STATE_USER_LOG
} MenuState;

typedef struct {
    MenuState currentState;
    bool actuatorActive;
    char users[MENU_MAX_USERS][MENU_USER_ID_LENGTH + 1];
    uint8_t userCount;
    char pendingUserId[MENU_USER_ID_LENGTH + 1];
    uint8_t pendingUserIdLength;
} MenuContext;

void menuInit(MenuContext *context);
void menuUpdate(MenuContext *context, char key);

#endif /* MENU_H */
