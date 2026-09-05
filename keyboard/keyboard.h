/**
 * @file keyboard.h
 * @brief Header file for 4x4 Matrix Keyboard State Machine
 * 
 * This module implements a finite state machine for handling a 4x4 matrix keyboard.
 * Responsible for reading key inputs and managing keyboard states.
 * 
 * @author Dev-2
 * @date 2026-06-02
 * @version 1.0
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

/* ============================================
 * KEYBOARD CONFIGURATION
 * ============================================ */

#define KEYBOARD_ROWS    4
#define KEYBOARD_COLS    4
#define TOTAL_KEYS       (KEYBOARD_ROWS * KEYBOARD_COLS)

/* ============================================
 * STATE MACHINE STATES
 * ============================================ */

/**
 * @enum KeyboardState
 * @brief States for keyboard state machine
 */
typedef enum {
    STATE_IDLE,           /**< Waiting for key press */
    STATE_KEY_PRESSED,    /**< Key detected */
    STATE_DEBOUNCE,       /**< Debouncing key input */
    STATE_KEY_HELD,       /**< Key being held down */
    STATE_KEY_RELEASED,   /**< Key released */
    STATE_ERROR           /**< Error state */
} KeyboardState_t;

/* ============================================
 * KEY DEFINITIONS
 * ============================================ */

/**
 * @enum KeyCode
 * @brief Key codes for 4x4 matrix keyboard
 * Layout:
 *  S1  S2  S3  S4
 *  S5  S6  S7  S8
 *  S9  S10 S11 S12
 *  S13 S14 S15 S16
 */
typedef enum {
    KEY_S1 = 1,   KEY_S2 = 2,   KEY_S3 = 3,   KEY_S4 = 4,
    KEY_S5 = 5,   KEY_S6 = 6,   KEY_S7 = 7,   KEY_S8 = 8,
    KEY_S9 = 9,   KEY_S10 = 10,   KEY_S11 = 11,   KEY_S12 = 12,
    KEY_S13 = 13,   KEY_S14 = 14,   KEY_S15 = 15,   KEY_S16 = 16,
    KEY_NONE = 0xFF  /**< No key pressed */
} KeyCode_t;

/* ============================================
 * KEYBOARD CONTEXT STRUCTURE
 * ============================================ */

/**
 * @struct KeyboardContext
 * @brief Context for keyboard state machine
 */
typedef struct {
    KeyboardState_t currentState;     /**< Current state */
    KeyboardState_t previousState;    /**< Previous state */
    KeyCode_t lastKeyPressed;         /**< Last key code detected */
    KeyCode_t currentKey;             /**< Current key being processed */
    uint32_t debounceCounter;         /**< Counter for debounce timing */
    uint32_t holdTime;                /**< Time key is being held */
    bool isKeyPressed;                /**< Flag: key currently pressed */
    uint8_t matrixBuffer[KEYBOARD_ROWS][KEYBOARD_COLS];  /**< Raw matrix data */
} KeyboardContext_t;

/* ============================================
 * FUNCTION PROTOTYPES
 * ============================================ */

/**
 * @brief Initialize keyboard state machine
 * @param ctx Pointer to keyboard context
 * @return true if initialization successful, false otherwise
 */
bool keyboard_init(KeyboardContext_t *ctx);

/**
 * @brief Update keyboard state machine (to be called periodically)
 * @param ctx Pointer to keyboard context
 * @return Current state of the keyboard
 */
KeyboardState_t keyboard_update(KeyboardContext_t *ctx);

/**
 * @brief Read matrix keyboard and return detected key
 * @param ctx Pointer to keyboard context
 * @return Key code if key detected, KEY_NONE if no key
 */
KeyCode_t keyboard_read(KeyboardContext_t *ctx);

/**
 * @brief Process key press event
 * @param ctx Pointer to keyboard context
 * @param key Key code that was pressed
 */
void keyboard_keyPressEvent(KeyboardContext_t *ctx, KeyCode_t key);

/**
 * @brief Process key release event
 * @param ctx Pointer to keyboard context
 * @param key Key code that was released
 */
void keyboard_keyReleaseEvent(KeyboardContext_t *ctx, KeyCode_t key);

/**
 * @brief Handle debouncing logic
 * @param ctx Pointer to keyboard context
 */
void keyboard_debounce(KeyboardContext_t *ctx);

/**
 * @brief Get current keyboard state
 * @param ctx Pointer to keyboard context
 * @return Current state
 */
KeyboardState_t keyboard_getState(KeyboardContext_t *ctx);

/**
 * @brief Get last pressed key
 * @param ctx Pointer to keyboard context
 * @return Last key code pressed
 */
KeyCode_t keyboard_getLastKey(KeyboardContext_t *ctx);

/**
 * @brief Reset keyboard state machine
 * @param ctx Pointer to keyboard context
 */
void keyboard_reset(KeyboardContext_t *ctx);

#endif /* KEYBOARD_H */
