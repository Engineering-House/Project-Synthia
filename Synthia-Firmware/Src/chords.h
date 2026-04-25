#ifndef CHORDS_H
#define CHORDS_H

#include <stdint.h>

typedef enum {
    CHORD_NONE = 0,
    CHORD_MAJOR,
    CHORD_MINOR,
    CHORD_DIMINISHED,
    CHORD_AUGMENTED,
    CHORD_MAJOR_7TH,
    CHORD_MINOR_7TH
} ChordType;

void chords_init(void);
void chords_scan(void);          // call from timer callback
void chords_get_current(uint8_t *root, ChordType *type);

#endif