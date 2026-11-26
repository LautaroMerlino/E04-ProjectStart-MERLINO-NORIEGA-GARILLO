/**
 * @file menu.c
 * @brief Implementation of Firmware Menu State Machine
 * 
 * Implements the finite state machine for the firmware menu system.
 * Handles menu navigation, state transitions, and user interactions.
 * 
 * @author Dev-3
 * @date 2026-06-02
 * @version 1.0
 */

#include "menu.h"
#include <string.h>

/* ============================================
 * MENU TIMEOUT CONFIGURATION
 * ============================================ */

#define MENU_TIMEOUT_MS     30000    /**< Menu timeout after 30 seconds of inactivity */
#define UPDATE_INTERVAL_MS  100      /**< Menu update interval */

/* ============================================
 * MENU ITEMS DEFINITIONS
 * ============================================ */

static const MenuItem_t mainMenuItems[] = {
    {1, "1. Read Sensors", MENU_STATE_READ_SENSORS, ACTION_READ_SENSORS},
    {2, "2. Activate Actuators", MENU_STATE_ACTIVATE_ACTUATORS, ACTION_ACTIVATE_ACTUATOR_1},
    {3, "3. Load New User", MENU_STATE_LOAD_USER, ACTION_LOAD_NEW_USER},
    {4, "4. Query User Log", MENU_STATE_QUERY_LOG, ACTION_QUERY_USER_LOG},
    {5, "5. Settings", MENU_STATE_SETTINGS, ACTION_NONE},
    {6, "6. Exit", MENU_STATE_EXIT, ACTION_EXIT_MENU}
};

static const uint8_t mainMenuItemCount = 6;

/* ============================================
 * STATIC FUNCTION PROTOTYPES
 * ============================================ */

/**
 * @brief Transition to new menu state
 * @param ctx Pointer to menu context
 * @param newState Target state
 */
static void menu_transitionState(MenuContext_t *ctx, MenuState_t newState);

/**
 * @brief State handler: MAIN menu
 * @param ctx Pointer to menu context
 */
static void menu_stateMain(MenuContext_t *ctx);

/**
 * @brief State handler: READ_SENSORS menu
 * @param ctx Pointer to menu context
 */
static void menu_stateReadSensors(MenuContext_t *ctx);

/**
 * @brief State handler: ACTIVATE_ACTUATORS menu
 * @param ctx Pointer to menu context
 */
static void menu_stateActivateActuators(MenuContext_t *ctx);

/**
 * @brief State handler: LOAD_USER menu
 * @param ctx Pointer to menu context
 */
static void menu_stateLoadUser(MenuContext_t *ctx);

/**
 * @brief State handler: QUERY_LOG menu
 * @param ctx Pointer to menu context
 */
static void menu_stateQueryLog(MenuContext_t *ctx);

/**
 * @brief State handler: PROCESSING state
 * @param ctx Pointer to menu context
 */
static void menu_stateProcessing(MenuContext_t *ctx);

/**
 * @brief State handler: CONFIRMATION state
 * @param ctx Pointer to menu context
 */
static void menu_stateConfirmation(MenuContext_t *ctx);

/**
 * @brief State handler: ERROR state
 * @param ctx Pointer to menu context
 */
static void menu_stateError(MenuContext_t *ctx);

/* ============================================
 * PUBLIC FUNCTION IMPLEMENTATIONS
 * ============================================ */

bool menu_init(MenuContext_t *ctx)
{
    if (ctx == NULL) {
        return false;
    }

    /* Initialize context structure */
    ctx->currentState = MENU_STATE_MAIN;
    ctx->previousState = MENU_STATE_MAIN;
    ctx->currentMenuIndex = 0;
    ctx->selectedItem = 0;
    ctx->menuDepth = 0;
    ctx->lastAction = ACTION_NONE;
    ctx->isProcessing = false;
    ctx->timeoutCounter = 0;
    ctx->updateCounter = 0;

    /* Clear menu stack */
    for (int i = 0; i < MAX_MENU_DEPTH; i++) {
        ctx->menuStack[i] = MENU_STATE_MAIN;
    }

    return true;
}

MenuState_t menu_update(MenuContext_t *ctx)
{
    if (ctx == NULL) {
        return MENU_STATE_ERROR;
    }

    /* Update timeout counter */
    ctx->updateCounter++;
    if (ctx->updateCounter >= (MENU_TIMEOUT_MS / UPDATE_INTERVAL_MS)) {
        ctx->updateCounter = 0;
        ctx->timeoutCounter++;
    }

    /* Check for timeout and auto-exit */
    if (ctx->timeoutCounter >= 1) {
        menu_reset(ctx);
        return MENU_STATE_MAIN;
    }

    /* Execute state handler based on current state */
    switch (ctx->currentState) {
        case MENU_STATE_MAIN:
            menu_stateMain(ctx);
            break;
        case MENU_STATE_READ_SENSORS:
            menu_stateReadSensors(ctx);
            break;
        case MENU_STATE_ACTIVATE_ACTUATORS:
            menu_stateActivateActuators(ctx);
            break;
        case MENU_STATE_LOAD_USER:
            menu_stateLoadUser(ctx);
            break;
        case MENU_STATE_QUERY_LOG:
            menu_stateQueryLog(ctx);
            break;
        case MENU_STATE_PROCESSING:
            menu_stateProcessing(ctx);
            break;
        case MENU_STATE_CONFIRMATION:
            menu_stateConfirmation(ctx);
            break;
        case MENU_STATE_ERROR:
            menu_stateError(ctx);
            break;
        case MENU_STATE_EXIT:
            menu_reset(ctx);
            return MENU_STATE_MAIN;
        default:
            ctx->currentState = MENU_STATE_ERROR;
            break;
    }

    return ctx->currentState;
}

void menu_handleKeyInput(MenuContext_t *ctx, uint8_t keyCode)
{
    if (ctx == NULL) {
        return;
    }

    /* Reset timeout on key input */
    ctx->timeoutCounter = 0;

    /* Handle key codes - map to menu actions */
    switch (keyCode) {
        case 2:  /* UP key or *key */
            menu_selectPrevious(ctx);
            break;
        case 8:  /* DOWN key or 8 key */
            menu_selectNext(ctx);
            break;
        case 0:  /* SELECT/ENTER key or 0 key */
            menu_confirmSelection(ctx);
            break;
        case 15: /* BACK/ESCAPE key or # key */
            menu_goBack(ctx);
            break;
        default:
            break;
    }
}

void menu_selectNext(MenuContext_t *ctx)
{
    if (ctx == NULL) {
        return;
    }

    if (ctx->selectedItem < (mainMenuItemCount - 1)) {
        ctx->selectedItem++;
    } else {
        ctx->selectedItem = 0;  /* Wrap around */
    }
}

void menu_selectPrevious(MenuContext_t *ctx)
{
    if (ctx == NULL) {
        return;
    }

    if (ctx->selectedItem > 0) {
        ctx->selectedItem--;
    } else {
        ctx->selectedItem = (mainMenuItemCount - 1);  /* Wrap around */
    }
}

MenuAction_t menu_confirmSelection(MenuContext_t *ctx)
{
    if (ctx == NULL) {
        return ACTION_NONE;
    }

    MenuAction_t action = mainMenuItems[ctx->selectedItem].action;
    MenuState_t nextState = mainMenuItems[ctx->selectedItem].targetState;

    if (ctx->menuDepth < MAX_MENU_DEPTH) {
        ctx->menuStack[ctx->menuDepth] = ctx->currentState;
        ctx->menuDepth++;
    }

    menu_transitionState(ctx, nextState);
    ctx->lastAction = action;

    return action;
}

void menu_goBack(MenuContext_t *ctx)
{
    if (ctx == NULL) {
        return;
    }

    if (ctx->menuDepth > 0) {
        ctx->menuDepth--;
        MenuState_t previousState = ctx->menuStack[ctx->menuDepth];
        menu_transitionState(ctx, previousState);
        ctx->selectedItem = 0;
    }
}

uint8_t menu_getMenuItems(MenuContext_t *ctx, MenuItem_t *items)
{
    if (ctx == NULL || items == NULL) {
        return 0;
    }

    /* Return main menu items for now - can be extended for submenu items */
    memcpy(items, mainMenuItems, sizeof(mainMenuItems));
    return mainMenuItemCount;
}

void menu_display(MenuContext_t *ctx)
{
    if (ctx == NULL) {
        return;
    }

    /* TODO: Implement LCD/Serial display logic here */
    /* This would display the current menu state and selected item */
}

bool menu_executeAction(MenuContext_t *ctx, MenuAction_t action)
{
    if (ctx == NULL) {
        return false;
    }

    switch (action) {
        case ACTION_READ_SENSORS:
            /* TODO: Implement sensor reading */
            break;
        case ACTION_ACTIVATE_ACTUATOR_1:
        case ACTION_ACTIVATE_ACTUATOR_2:
            /* TODO: Implement actuator control */
            break;
        case ACTION_LOAD_NEW_USER:
            /* TODO: Implement user loading */
            break;
        case ACTION_QUERY_USER_LOG:
            /* TODO: Implement user log query */
            break;
        case ACTION_EXIT_MENU:
            menu_reset(ctx);
            break;
        default:
            return false;
    }

    return true;
}

MenuState_t menu_getState(MenuContext_t *ctx)
{
    if (ctx == NULL) {
        return MENU_STATE_ERROR;
    }

    return ctx->currentState;
}

void menu_reset(MenuContext_t *ctx)
{
    if (ctx == NULL) {
        return;
    }

    menu_init(ctx);
}

/* ============================================
 * STATIC FUNCTION IMPLEMENTATIONS
 * ============================================ */

static void menu_transitionState(MenuContext_t *ctx, MenuState_t newState)
{
    if (ctx == NULL) {
        return;
    }

    ctx->previousState = ctx->currentState;
    ctx->currentState = newState;
    ctx->selectedItem = 0;  /* Reset selection on state change */
}

static void menu_stateMain(MenuContext_t *ctx)
{
    /**
     * STATE: MAIN
     * Description: Main menu display
     * Transitions:
     *   - User selection -> corresponding submenu
     *   - No user input (timeout) -> EXIT
     */

    menu_display(ctx);
}

static void menu_stateReadSensors(MenuContext_t *ctx)
{
    /**
     * STATE: READ_SENSORS
     * Description: Display sensor readings
     * Transitions:
     *   - Back selected -> MAIN
     *   - Processing complete -> MAIN
     */

    ctx->isProcessing = true;
    menu_executeAction(ctx, ACTION_READ_SENSORS);
    ctx->isProcessing = false;
}

static void menu_stateActivateActuators(MenuContext_t *ctx)
{
    /**
     * STATE: ACTIVATE_ACTUATORS
     * Description: Actuator control submenu
     * Transitions:
     *   - Selection -> CONFIRMATION
     *   - Back selected -> MAIN
     */

    menu_display(ctx);
}

static void menu_stateLoadUser(MenuContext_t *ctx)
{
    /**
     * STATE: LOAD_USER
     * Description: Load new user form
     * Transitions:
     *   - Input complete -> CONFIRMATION
     *   - Back selected -> MAIN
     */

    menu_display(ctx);
}

static void menu_stateQueryLog(MenuContext_t *ctx)
{
    /**
     * STATE: QUERY_LOG
     * Description: Display user log/history
     * Transitions:
     *   - Back selected -> MAIN
     */

    menu_display(ctx);
}

static void menu_stateProcessing(MenuContext_t *ctx)
{
    /**
     * STATE: PROCESSING
     * Description: Processing user command
     * Transitions:
     *   - Processing complete -> CONFIRMATION
     *   - Error -> ERROR
     */

    if (!ctx->isProcessing) {
        menu_transitionState(ctx, MENU_STATE_CONFIRMATION);
    }
}

static void menu_stateConfirmation(MenuContext_t *ctx)
{
    /**
     * STATE: CONFIRMATION
     * Description: Show confirmation and return to main
     * Transitions:
     *   - Timeout -> MAIN
     *   - User input -> MAIN
     */

    menu_transitionState(ctx, MENU_STATE_MAIN);
}

static void menu_stateError(MenuContext_t *ctx)
{
    /**
     * STATE: ERROR
     * Description: Error state - recovery
     * Transitions:
     *   - Reset triggered -> MAIN
     */

    menu_reset(ctx);
}
