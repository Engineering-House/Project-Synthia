/**
* @file AudioGenerator.h
* @brief A simple library for generating the values used with an audio DAC
* @author Nathan Kiehl
* @copyright Copyright (c) 2026 Nathan Kiehl. Licensed under the MIT License.
*/

#ifndef AudioGenerator_H

#define AudioGenerator_H

/**
* @brief Represents every note in half steps from [C-B]
*
* When creating a chord, there is a base note that the rest
* of the formula bases it on. The N at the end signifies
* a natural note while the S indicates a sharp. All acidentals
* are indicated with a sharp for convienience.
*/
typedef enum {
	NOTE_CN,
	NOTE_CS,
	NOTE_DN,
	NOTE_DS,
	NOTE_EN,
	NOTE_FN,
	NOTE_FS,
	NOTE_GN,
	NOTE_GS,
	NOTE_AN,
	NOTE_AS,
	NOTE_BN
} RootNote;

/**
* @brief Represents a type of chord
* 
* This is what determines how the chord is created from the root note.
* Major: Root + Major 3rd + Perfect 5th
* Minor: Root + Minor 3rd + Perfect 5th
* Augmented: Root + Major 3rd + Augmented 5th
* Diminished: Root + Minor 3rd + Diminshed 5th
*/
typedef enum {
	CHORD_MAJOR,
	CHORD_MINOR,
	CHORD_AUGMENTED,
	CHORD_DIMINSHED
} ChordType;

/**
* @brief Represents the wave type to be used in a given chord
*/
typedef enum {
	SQUARE_WAVE,
	TRIANGLE_WAVE,
	SAW_WAVE
} WaveType;

int generateChordValue_Unsigned8Bit(WaveType wavetype, ChordType chordtype, RootNote rootnote, int rootNoteOctave, float timeMS);

#endif // !AudioGenerator_H
