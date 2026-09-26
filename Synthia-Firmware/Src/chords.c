#include "chords.h"
#include "stm32g4xx_hal.h"

// Row output pins
#define MAJOR_ROW_PORT      GPIOA
#define MAJOR_ROW_PIN       GPIO_PIN_7
#define MINOR_ROW_PORT      GPIOB
#define MINOR_ROW_PIN       GPIO_PIN_0
#define SEVENTH_ROW_PORT    GPIOA
#define SEVENTH_ROW_PIN     GPIO_PIN_9

// Column input pins
#define COL0_PIN            GPIO_PIN_4
#define COL1_PIN            GPIO_PIN_5
#define COL2_PIN            GPIO_PIN_6
#define COL3_PIN            GPIO_PIN_7
#define COL_PORT            GPIOB
#define COL_PINS            (COL0_PIN | COL1_PIN | COL2_PIN | COL3_PIN)

#define NUM_ROWS 3
#define NUM_COLS 4
#define DEBOUNCE_THRESH 3   // number of consecutive identical readings

static uint8_t raw_state[NUM_ROWS];         // one bitmask per row
static uint8_t stable_state[NUM_ROWS];      // debounced bitmask per row
static uint8_t debounce_cnt[NUM_ROWS][NUM_COLS]; // counter for each button
static uint8_t current_root = 0;
static ChordType current_type = CHORD_NONE;

// Helper to set all row outputs low
static void clear_all_rows(void) {
    HAL_GPIO_WritePin(MAJOR_ROW_PORT, MAJOR_ROW_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MINOR_ROW_PORT, MINOR_ROW_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEVENTH_ROW_PORT, SEVENTH_ROW_PIN, GPIO_PIN_RESET);
}

// Activate one row and read columns
static uint8_t read_row(uint8_t row) {
    clear_all_rows();
    switch (row) {
        case 0:
            HAL_GPIO_WritePin(MAJOR_ROW_PORT, MAJOR_ROW_PIN, GPIO_PIN_SET);
            break;
        case 1:
            HAL_GPIO_WritePin(MINOR_ROW_PORT, MINOR_ROW_PIN, GPIO_PIN_SET);
            break;
        case 2:
            HAL_GPIO_WritePin(SEVENTH_ROW_PORT, SEVENTH_ROW_PIN, GPIO_PIN_SET);
            break;
        default:
            return 0;
    }
    // Small settling delay (a few instructions) – not strictly necessary
    __asm volatile ("nop\n nop\n nop\n");

    uint16_t ports = COL_PORT->IDR;
    uint8_t bits = 0;
    if (ports & COL0_PIN) bits |= (1 << 0);
    if (ports & COL1_PIN) bits |= (1 << 1);
    if (ports & COL2_PIN) bits |= (1 << 2);
    if (ports & COL3_PIN) bits |= (1 << 3);
    return bits;
}

// Debounce raw reading into stable_state
static void debounce(void) {
    for (int row = 0; row < NUM_ROWS; row++) {
        uint8_t raw = raw_state[row];
        for (int col = 0; col < NUM_COLS; col++) {
            uint8_t raw_bit = (raw >> col) & 0x01;
            if (raw_bit == ((stable_state[row] >> col) & 0x01)) {
                // reading matches stable state → reset counter
                debounce_cnt[row][col] = 0;
            } else {
                // different → increment, latch if threshold reached
                debounce_cnt[row][col]++;
                if (debounce_cnt[row][col] >= DEBOUNCE_THRESH) {
                    if (raw_bit)
                        stable_state[row] |= (1 << col);
                    else
                        stable_state[row] &= ~(1 << col);
                    debounce_cnt[row][col] = 0;
                }
            }
        }
    }
}

// Determine chord from stable_state, update global root/type
static void detect_chord(void) {
    // priority: first column (col0) with a valid chord wins
    for (int col = 0; col < NUM_COLS; col++) {
        int maj  = (stable_state[0] >> col) & 1;
        int min  = (stable_state[1] >> col) & 1;
        int sev  = (stable_state[2] >> col) & 1;
        ChordType type = CHORD_NONE;

        if (maj && min && sev)       type = CHORD_AUGMENTED;
        else if (maj && min && !sev) type = CHORD_DIMINISHED;
        else if (maj && sev && !min) type = CHORD_MAJOR_7TH;
        else if (min && sev && !maj) type = CHORD_MINOR_7TH;
        else if (maj && !min && !sev) type = CHORD_MAJOR;
        else if (min && !maj && !sev) type = CHORD_MINOR;
        // ignore sev alone

        if (type != CHORD_NONE) {
            current_root = col;
            current_type = type;
            // Optionally call a function to update DAC immediately
            // update_dac_based_on_chord(col, type);
            return;   // stop at first valid column
        }
    }
    // No chord pressed
    current_type = CHORD_NONE;
}

void chords_init(void) {
    // GPIOs already initialised by CubeMX; just clear outputs
    clear_all_rows();
    // Clear arrays
    for (int r = 0; r < NUM_ROWS; r++) {
        raw_state[r] = 0;
        stable_state[r] = 0;
        for (int c = 0; c < NUM_COLS; c++)
            debounce_cnt[r][c] = 0;
    }
    current_type = CHORD_NONE;
    current_root = 0;
}

void chords_scan(void) {
    // Scan each row
    for (int row = 0; row < NUM_ROWS; row++)
        raw_state[row] = read_row(row);

    // Turn off all rows after scan to save power
    clear_all_rows();

    debounce();          // update stable_state
    detect_chord();      // update current_root/type
}

void chords_get_current(uint8_t *root, ChordType *type) {
    *root = current_root;
    *type = current_type;
}