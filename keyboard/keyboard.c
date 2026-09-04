/**
 * @file keyboard.c
 * @brief Implementation of 4x4 Matrix Keyboard State Machine
 * 
 * Implements the finite state machine for handling keyboard matrix input.
 * Manages debouncing, state transitions, and key event generation.
 * 
 * @author Dev-2
 * @date 2026-06-02
 * @version 1.0
 */

#include "keyboard.h"

/* ============================================
 * CONFIGURATION CONSTANTS
 * ============================================ */

#define DEBOUNCE_TIME_MS    20    /**< Debounce delay in milliseconds */
#define HOLD_TIME_MS        500   /**< Time to consider key "held" */

/* ============================================
 * STATIC FUNCTION PROTOTYPES
 * ============================================ */

/**
 * @brief Scan the keyboard matrix for key presses
 * @param ctx Pointer to keyboard context
 * @return Key code if detected, KEY_NONE otherwise
 */
static KeyCode_t keyboard_scanMatrix(KeyboardContext_t *ctx);

/**
 * @brief Transition to new state
 * @param ctx Pointer to keyboard context
 * @param newState Target state
 */
static void keyboard_transitionState(KeyboardContext_t *ctx, KeyboardState_t newState);

/**
 * @brief State handler: IDLE state
 * @param ctx Pointer to keyboard context
 */
static void keyboard_stateIdle(KeyboardContext_t *ctx);

/**
 * @brief State handler: KEY_PRESSED state
 * @param ctx Pointer to keyboard context
 */
static void keyboard_stateKeyPressed(KeyboardContext_t *ctx);

/**
 * @brief State handler: DEBOUNCE state
 * @param ctx Pointer to keyboard context
 */
static void keyboard_stateDebounce(KeyboardContext_t *ctx);

/**
 * @brief State handler: KEY_HELD state
 * @param ctx Pointer to keyboard context
 */
static void keyboard_stateKeyHeld(KeyboardContext_t *ctx);

/**
 * @brief State handler: KEY_RELEASED state
 * @param ctx Pointer to keyboard context
 */
static void keyboard_stateKeyReleased(KeyboardContext_t *ctx);

/* ============================================
 * PUBLIC FUNCTION IMPLEMENTATIONS
 * ============================================ */

bool keyboard_init(KeyboardContext_t *ctx)
{
    if (ctx == NULL) {
        return false;
    }

    /* Initialize context structure */
    ctx->currentState = STATE_IDLE;
    ctx->previousState = STATE_IDLE;
    ctx->lastKeyPressed = KEY_NONE;
    ctx->currentKey = KEY_NONE;
    ctx->debounceCounter = 0;
    ctx->holdTime = 0;
    ctx->isKeyPressed = false;

    /* Clear matrix buffer */
    for (int i = 0; i < KEYBOARD_ROWS; i++) {
        for (int j = 0; j < KEYBOARD_COLS; j++) {
            ctx->matrixBuffer[i][j] = 0;
        }
    }

    return true;
}

KeyboardState_t keyboard_update(KeyboardContext_t *ctx)
{
    if (ctx == NULL) {
        return STATE_ERROR;
    }

    /* Execute state handler based on current state */
    switch (ctx->currentState) {
        case STATE_IDLE:
            keyboard_stateIdle(ctx);
            break;
        case STATE_KEY_PRESSED:
            keyboard_stateKeyPressed(ctx);
            break;
        case STATE_DEBOUNCE:
            keyboard_stateDebounce(ctx);
            break;
        case STATE_KEY_HELD:
            keyboard_stateKeyHeld(ctx);
            break;
        case STATE_KEY_RELEASED:
            keyboard_stateKeyReleased(ctx);
            break;
        case STATE_ERROR:
            keyboard_reset(ctx);
            break;
        default:
            ctx->currentState = STATE_ERROR;
            break;
    }

    return ctx->currentState;
}

KeyCode_t keyboard_read(KeyboardContext_t *ctx)
{
    if (ctx == NULL) {
        return KEY_NONE;
    }

    return keyboard_scanMatrix(ctx);
}

void keyboard_keyPressEvent(KeyboardContext_t *ctx, KeyCode_t key)
{
    if (ctx == NULL) {
        return;
    }

    ctx->currentKey = key;
    ctx->isKeyPressed = true;
    ctx->debounceCounter = 0;
    ctx->holdTime = 0;
}

void keyboard_keyReleaseEvent(KeyboardContext_t *ctx, KeyCode_t key)
{
    if (ctx == NULL) {
        return;
    }

    if (ctx->currentKey == key) {
        ctx->lastKeyPressed = key;
        ctx->isKeyPressed = false;
        ctx->currentKey = KEY_NONE;
        ctx->debounceCounter = 0;
        ctx->holdTime = 0;
    }
}

void keyboard_debounce(KeyboardContext_t *ctx)
{
    if (ctx == NULL) {
        return;
    }

    /* Debounce logic implementation */
    if (ctx->debounceCounter < DEBOUNCE_TIME_MS) {
        ctx->debounceCounter++;
    }
}

KeyboardState_t keyboard_getState(KeyboardContext_t *ctx)
{
    if (ctx == NULL) {
        return STATE_ERROR;
    }

    return ctx->currentState;
}

KeyCode_t keyboard_getLastKey(KeyboardContext_t *ctx)
{
    if (ctx == NULL) {
        return KEY_NONE;
    }

    return ctx->lastKeyPressed;
}

void keyboard_reset(KeyboardContext_t *ctx)
{
    if (ctx == NULL) {
        return;
    }

    keyboard_init(ctx);
}

/* ============================================
 * STATIC FUNCTION IMPLEMENTATIONS
 * ============================================ */

static KeyCode_t keyboard_scanMatrix(KeyboardContext_t *ctx)
{
    /* TODO: Implement actual matrix scanning logic */
    /* This should read GPIO pins and detect which key is pressed */
    /* For now, returning KEY_NONE as placeholder */
    return KEY_NONE;
}

static void keyboard_transitionState(KeyboardContext_t *ctx, KeyboardState_t newState)
{
    if (ctx == NULL) {
        return;
    }

    ctx->previousState = ctx->currentState;
    ctx->currentState = newState;
}

static void keyboard_stateIdle(KeyboardContext_t *ctx)
{
    /**
     * STATE: IDLE
     * Description: Waiting for any key press
     * Transitions:
     *   - Press detected -> STATE_KEY_PRESSED
     */

    KeyCode_t key = keyboard_scanMatrix(ctx);

    if (key != KEY_NONE) {
        keyboard_keyPressEvent(ctx, key);
        keyboard_transitionState(ctx, STATE_KEY_PRESSED);
    }
}

static void keyboard_stateKeyPressed(KeyboardContext_t *ctx)
{
    /**
     * STATE: KEY_PRESSED
     * Description: Initial key press detected, begin debounce
     * Transitions:
     *   - Debounce timeout -> STATE_DEBOUNCE
     *   - Key released -> STATE_IDLE
     */

    keyboard_debounce(ctx);

    if (ctx->debounceCounter >= DEBOUNCE_TIME_MS) {
        keyboard_transitionState(ctx, STATE_DEBOUNCE);
    }

    KeyCode_t key = keyboard_scanMatrix(ctx);
    if (key == KEY_NONE) {
        keyboard_keyReleaseEvent(ctx, ctx->currentKey);
        keyboard_transitionState(ctx, STATE_IDLE);
    }
}

static void keyboard_stateDebounce(KeyboardContext_t *ctx)
{
    /**
     * STATE: DEBOUNCE
     * Description: Debounce complete, key press confirmed
     * Transitions:
     *   - Proceed -> STATE_KEY_HELD
     *   - Key released -> STATE_IDLE
     */

    keyboard_transitionState(ctx, STATE_KEY_HELD);
}

static void keyboard_stateKeyHeld(KeyboardContext_t *ctx)
{
    /**
     * STATE: KEY_HELD
     * Description: Key is being held down
     * Transitions:
     *   - Key released -> STATE_KEY_RELEASED
     */

    ctx->holdTime++;

    KeyCode_t key = keyboard_scanMatrix(ctx);
    if (key == KEY_NONE) {
        keyboard_keyReleaseEvent(ctx, ctx->currentKey);
        keyboard_transitionState(ctx, STATE_KEY_RELEASED);
    }
}

static void keyboard_stateKeyReleased(KeyboardContext_t *ctx)
{
    /**
     * STATE: KEY_RELEASED
     * Description: Key was released, processing complete
     * Transitions:
     *   - Proceed -> STATE_IDLE
     */

    keyboard_transitionState(ctx, STATE_IDLE);
}
