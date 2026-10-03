/*******************************************************************************
 * @file        AudioGenerator.c
 * @brief       Provides functions to generate audio output for an 8-bit DAC.
 *
 * @details     This file generates audio values for an eight bit DAC. There are
 *				a variety of audio output styles available. Currently the
 *				available styles are square wave, saw wave, and triangle wave.
 *
 * @author      Nathan Kiehl
 * @date        2026-10-01
 * @version     1.0.0
 ******************************************************************************/

/*******************************************************************************
 * TODO:
 *	- Add dither to audio scaling (Potentially)
 *	- Add support for chords with varying wave styles  
 ******************************************************************************/


#include <math.h>

#include "AudioGenerator.h"


static const float BASE_NOTE_C0 = 16.352f;

/**
 * @brief Generates a note frequency for a given note
 *
 * Given the octave and note, produces the frequency.
 * The starting note for any given octave is C.
 * The note can be defined using @ref RootNote or 0-11. 
 *
 * @param octave	  The octave that the generated frequency comes from (may be 0).
 * @param note        The integer representing a note C-B -> 0-11.
 * @return float      The resulting frequency of the note.
 */
static float noteFrequency(int octave, RootNote note) {
	return BASE_NOTE_C0 * pow(2.0, octave + (note/12.0));
}

/**
 * @brief Generates a note frequency relative to another frequency
 *
 * Given a frequency, a note from a specified number of halfsteps up
 * is generated.
 *
 * @param frequency	  The freequency of the original note
 * @param halfsteps   The number of halfsteps above the origial frequency
 * @return float      The resulting frequency of the note
 */
static float relativeFrequencyUp(float frequency, int halfsteps) {
	return frequency * pow(2.0, halfsteps/12.0);
}

/**
* @brief Finds magnitude of square wave, assuming a range of [0.0,1.0]
* 
* Determine the magnitude of a square wave from the current clock time in
* milliseconds and the octave and note. The wave begins high. A high value
* is represented by an output of 1.0f and a low value by 0.0f.
* 
* @param timeMS		The current time in ms
* @param octave		The octave the note is located in
* @param note		The integer representing a note C-B -> 0-11
* @return float		The current magnitude of the note 0.0 -> 1.0
*/
static float squareWaveMagnitude_Note(float timeMS, int octave, RootNote note) {
	float frequency = noteFrequency(octave, note);
	return squareWaveMagnitude_Frequency(timeMS, frequency);
}

/**
* @brief Finds magnitude of square wave, assuming a range of [0.0,1.0]
*
* Determine the magnitude of a square wave from the current clock time in
* milliseconds and the frequency. The wave begins high. A high value
* is represented by an output of 1.0f and a low value by 0.0f.
*
* @param timeMS		The current time in ms
* @param frequency	The provided frequency of the note
* @return float		The current magnitude of the note 0.0 -> 1.0
*/
static float squareWaveMagnitude_Frequency(float timeMS, float frequency) {
	float periodMS = 1000.0f / frequency;
	if (fmod(timeMS, periodMS) < periodMS / 2.0f) {
		return 1.0f;
	}
	return 0.0f;
}

/**
* @brief Finds magnitude of triangle wave, assuming a range of [0.0,1.0]
*
* Determine the magnitude of a triangle wave from the current clock time in
* milliseconds and the note. The wave begins low. A high value
* is represented by an output of 1.0f and a low value by 0.0f.
*
* @param timeMS		The current time in ms
* @param octave		The octave the note is located in
* @param note		The integer representing a note C-B -> 0-11
* @return float		The current magnitude of the note 0.0 -> 1.0
*/
static float triangleWaveMagnitude_Note(float timeMS, int octave, RootNote note) {
	float frequency = noteFrequency(octave, note);
	return triangleWaveMagnitude_Frequency(timeMS, frequency);
}

/**
* @brief Finds magnitude of saw wave, assuming a range of [0.0,1.0]
*
* Determine the magnitude of a saw wave from the current clock time in
* milliseconds and the frequency. The wave begins low. A high value
* is represented by an output of 1.0f and a low value by 0.0f.
*
* @param timeMS		The current time in ms
* @param frequency	The frequency of the note
* @return float		The current magnitude of the note 0.0 -> 1.0
*/
static float sawWaveMagnitude_Frequency(float timeMS, float frequency) {
	float periodMS = 1000.0f / frequency;

	return fmod(timeMS, periodMS) / periodMS;
}


/**
* @brief Finds magnitude of saw wave, assuming a range of [0.0,1.0]
*
* Determine the magnitude of a saw wave from the current clock time in
* milliseconds and the note. The wave begins low. A high value
* is represented by an output of 1.0f and a low value by 0.0f.
*
* @param timeMS		The current time in ms
* @param octave		The octave the note is located in
* @param note		The integer representing a note C-B -> 0-11
* @return float		The current magnitude of the note 0.0 -> 1.0
*/
static float sawWaveMagnitude_Note(float timeMS, int octave, RootNote note) {
	float frequency = noteFrequency(octave, note);
	return sawWaveMagnitude_Frequency(timeMS, frequency);
}

/**
* @brief Finds magnitude of triangle wave, assuming a range of [0.0,1.0]
*
* Determine the magnitude of a triangle wave from the current clock time in
* milliseconds and the frequency. The wave begins low. A high value
* is represented by an output of 1.0f and a low value by 0.0f.
*
* @param timeMS		The current time in ms
* @param frequency	The frequency of the note
* @return float		The current magnitude of the note 0.0 -> 1.0
*/
static float triangleWaveMagnitude_Frequency(float timeMS, float frequency) {
	float periodMS = 1000.0f / frequency;
	float halfPeriod = periodMS / 2.0;
	float tSubPeriodMS = fmod(timeMS, periodMS);

	if (tSubPeriodMS < halfPeriod) {
		return tSubPeriodMS / halfPeriod;
	}
	return 1.0f - ((tSubPeriodMS - halfPeriod) / halfPeriod);
}

/**
 * @brief Generates an 8-bit unsigned value for audio output of a chord
 *
 * Taking various information about the note to be produced, generate audio
 * determines the output value for the given waveform
 *
 * @param wavetype	  The type of wave used to generate the chord.
 * @param chordtype	  The type of chord to create.
 * @param rootnote	  The root to use in the chord.
 * @return int		  The resulting value for input into a DAC [0-255].
 */
int generateChordValue_Unsigned8Bit(WaveType waveType, ChordType chordType, RootNote rootNote, int rootNoteOctave, float timeMS) {	

	int noteTwoHalfSteps = 0;
	int noteThreeHalfSteps = 0;
	
	switch (chordType) {
		case CHORD_MAJOR:
			noteTwoHalfSteps = 4;
			noteThreeHalfSteps = 7;
			break;
		case CHORD_MINOR:
			noteTwoHalfSteps = 3;
			noteThreeHalfSteps = 7;
			break;
		case CHORD_AUGMENTED:
			noteTwoHalfSteps = 4;
			noteThreeHalfSteps = 8;
			break;
		case CHORD_DIMINSHED:
			noteTwoHalfSteps = 3;
			noteThreeHalfSteps = 6;
			break;
		default:
			return 128; //Sets DAC to silent if there is an invalid chordtype
	}

	float frequencyOne = noteFrequency(rootNoteOctave, rootNote);
	float frequencyTwo = relativeFrequencyUp(frequencyOne, noteTwoHalfSteps);
	float frequencyThree = relativeFrequencyUp(frequencyOne, noteThreeHalfSteps);

	float magnitudeOne, magnitudeTwo, magnitudeThree;


	switch (waveType) {
		case SQUARE_WAVE:
			magnitudeOne = squareWaveMagnitude_Note(timeMS, rootNoteOctave, rootNote);
			magnitudeTwo = squareWaveMagnitude_Frequency(timeMS, frequencyTwo);
			magnitudeThree = squareWaveMagnitude_Frequency(timeMS, frequencyThree);
			break;
		case SAW_WAVE:
			magnitudeOne = sawWaveMagnitude_Note(timeMS, rootNoteOctave, rootNote);
			magnitudeTwo = sawWaveMagnitude_Frequency(timeMS, frequencyTwo);
			magnitudeThree = sawWaveMagnitude_Frequency(timeMS, frequencyThree);
			break;
		case TRIANGLE_WAVE:
			magnitudeOne = triangleWaveMagnitude_Note(timeMS, rootNoteOctave, rootNote);
			magnitudeTwo = triangleWaveMagnitude_Frequency(timeMS, frequencyTwo);
			magnitudeThree = triangleWaveMagnitude_Frequency(timeMS, frequencyThree);
			break;
		default:
			return 128; //Sets DAC to silent if there is an invalid wavetype
	}

	int eightBitOutput = (int)round(255.0 * ((magnitudeOne + magnitudeTwo + magnitudeThree) / 3.0f));
	if (eightBitOutput > 255) { eightBitOutput = 255; }
	else if (eightBitOutput < 0) { eightBitOutput = 0; }
	return eightBitOutput;
	
}