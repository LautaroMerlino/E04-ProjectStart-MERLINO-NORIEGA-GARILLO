/**
 * @file menu.h
 * @brief Header file for Firmware Menu State Machine
 * 
 * This module implements a finite state machine for the firmware menu system.
 * Manages navigation between different menu states and handles user interaction.
 * 
 * @author Dev-3
 * @date 2026-06-02
 * @version 1.0
 */

#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include <stdbool.h>

/* ============================================
 * MENU CONFIGURATION
 * ============================================ */

#define MAX_MENU_ITEMS      10    /**< Maximum menu items per menu */
#define MAX_MENU_DEPTH      5     /**< Maximum submenu nesting depth */

/* ============================================
 * MENU STATE MACHINE STATES
 * ============================================ */

/**
 * @enum MenuState
 * @brief States for menu state machine
 */
typedef enum {
    MENU_STATE_MAIN,              /**< Main menu */
    MENU_STATE_READ_SENSORS,      /**< Read sensors submenu */
    MENU_STATE_ACTIVATE_ACTUATORS, /**< Activate actuators submenu */
    MENU_STATE_LOAD_USER,         /**< Load new user submenu */
    MENU_STATE_QUERY_LOG,         /**< Query user log submenu */
    MENU_STATE_SETTINGS,          /**< Settings submenu */
    MENU_STATE_PROCESSING,        /**< Processing command state */
    MENU_STATE_CONFIRMATION,      /**< Confirmation state */
    MENU_STATE_ERROR,             /**< Error state */
    MENU_STATE_EXIT                /**< Exit menu state */
} MenuState_t;

/* ============================================
 * MENU ACTION TYPES
 * ============================================ */

/**
 * @enum MenuAction
 * @brief Actions that can be triggered from menu
 */
typedef enum {
    ACTION_NONE = 0,
    ACTION_READ_SENSORS,
    ACTION_ACTIVATE_ACTUATOR_1,
    ACTION_ACTIVATE_ACTUATOR_2,
    ACTION_LOAD_NEW_USER,
    ACTION_QUERY_USER_LOG,
    ACTION_DISPLAY_TIME,
    ACTION_GO_BACK,
    ACTION_EXIT_MENU,
    ACTION_CONFIRM,
    ACTION_CANCEL
} MenuAction_t;

/* ============================================
 * MENU ITEM STRUCTURE
 * ============================================ */

/**
 * @struct MenuItem
 * @brief Menu item definition
 */
typedef struct {
    uint8_t id;               /**< Item ID */
    const char *label;        /**< Display label */
    MenuState_t targetState;  /**< Next state if selected */
    MenuAction_t action;      /**< Action to execute */
} MenuItem_t;

/* ============================================
 * MENU CONTEXT STRUCTURE
 * ============================================ */

/**
 * @struct MenuContext
 * @brief Context for menu state machine
 */
typedef struct {
    MenuState_t currentState;        /**< Current menu state */
    MenuState_t previousState;       /**< Previous menu state (for back navigation) */
    uint8_t currentMenuIndex;        /**< Index in current menu */
    uint8_t selectedItem;            /**< Currently selected item */
    uint8_t menuDepth;               /**< Current menu nesting level */
    MenuState_t menuStack[MAX_MENU_DEPTH];  /**< Stack for menu navigation */
    MenuAction_t lastAction;         /**< Last action triggered */
    bool isProcessing;               /**< Flag: command is being processed */
    uint32_t timeoutCounter;         /**< Timeout counter for auto-exit */
    uint32_t updateCounter;          /**< Counter for menu updates */
} MenuContext_t;

/* ============================================
 * FUNCTION PROTOTYPES
 * ============================================ */

/**
 * @brief Initialize menu state machine
 * @param ctx Pointer to menu context
 * @return true if initialization successful, false otherwise
 */
bool menu_init(MenuContext_t *ctx);

/**
 * @brief Update menu state machine (to be called periodically)
 * @param ctx Pointer to menu context
 * @return Current state of the menu
 */
MenuState_t menu_update(MenuContext_t *ctx);

/**
 * @brief Handle user input from keyboard
 * @param ctx Pointer to menu context
 * @param keyCode Key code received from keyboard
 */
void menu_handleKeyInput(MenuContext_t *ctx, uint8_t keyCode);

/**
 * @brief Navigate to next menu item
 * @param ctx Pointer to menu context
 */
void menu_selectNext(MenuContext_t *ctx);

/**
 * @brief Navigate to previous menu item
 * @param ctx Pointer to menu context
 */
void menu_selectPrevious(MenuContext_t *ctx);

/**
 * @brief Confirm selection and execute action
 * @param ctx Pointer to menu context
 * @return Action that was triggered
 */
MenuAction_t menu_confirmSelection(MenuContext_t *ctx);

/**
 * @brief Go back to previous menu
 * @param ctx Pointer to menu context
 */
void menu_goBack(MenuContext_t *ctx);

/**
 * @brief Get currently displayed menu items
 * @param ctx Pointer to menu context
 * @param items Pointer to menu item array
 * @return Number of items in current menu
 */
uint8_t menu_getMenuItems(MenuContext_t *ctx, MenuItem_t *items);

/**
 * @brief Display current menu on output device (LCD, Serial, etc.)
 * @param ctx Pointer to menu context
 */
void menu_display(MenuContext_t *ctx);

/**
 * @brief Execute menu action
 * @param ctx Pointer to menu context
 * @param action Action to execute
 * @return true if action executed successfully
 */
bool menu_executeAction(MenuContext_t *ctx, MenuAction_t action);

/**
 * @brief Get current menu state
 * @param ctx Pointer to menu context
 * @return Current state
 */
MenuState_t menu_getState(MenuContext_t *ctx);

/**
 * @brief Reset menu to main state
 * @param ctx Pointer to menu context
 */
void menu_reset(MenuContext_t *ctx);

#endif /* MENU_H */
