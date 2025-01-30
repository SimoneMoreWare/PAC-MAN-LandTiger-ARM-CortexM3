#include "game_sound.h"
#include "game_melodies.h"
#include "../timer/timer.h"

unsigned short currentNote = 0;

const unsigned short SinTable[45] = {
	410, 467, 523, 576, 627, 673, 714, 749, 778,
	799, 813, 819, 817, 807, 789, 764, 732, 694, 
	650, 602, 550, 495, 438, 381, 324, 270, 217,
	169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
	20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};


void playNote(NOTE note)
{
	if(note.freq != REST)
	{
		reset_timer(2);
		init_timer(2, 0, 0, 3, note.freq*AMPLIFIER*VOLUME);	
		enable_timer(2);
	}
	reset_timer(3);
	init_timer(3, 0, 0, 7, note.duration);
	enable_timer(3);
}

BOOL isNotePlaying(void)
{
	return ((LPC_TIM2->TCR != 0) || (LPC_TIM3->TCR != 0));
}

void play_tone_single_note(){
		static int sineticks=0;
		/* DAC management */	
		static int currentValue; 
		currentValue = SinTable[sineticks];
		currentValue -= 410;
		currentValue /= 1;
		currentValue += 410;
		LPC_DAC->DACR = currentValue <<6;
		sineticks++;
		if(sineticks>=45) sineticks=0;
}

void play_tone_single_note_duration(){
		LPC_TIM2->TCR = 0;
		disable_timer(2);
}

/**
 * Plays the next tone for the currently active track.
 * 
 * This function handles track playback logic, ensuring that notes are played in sequence, 
 * with a pause (defined by `UPTICKS`) between each note. When the track finishes, it either 
 * restarts the track or switches to the previously active track, depending on the track's settings.
 */
void play_tone_all_track() {
    // Static variable to track the number of ticks since the last note was played
    static unsigned char tickCounter = 0;

    // Get the track currently marked as playing
    const TRACK* activeTrack = get_track_by_ID(get_current_track());

    // If no note is currently playing, handle note playback logic
    if (!isNotePlaying()) {
        ++tickCounter;

        // If the tick counter reaches the required number of ticks, play the next note
        if (tickCounter == UPTICKS) {
            tickCounter = 0; // Reset the tick counter
            playNote(activeTrack->notes[currentNote++]); // Play the current note and increment the note index
        }
    }

    // Check if the end of the track has been reached
    if (currentNote == activeTrack->dimArray) {
        if (activeTrack->toRestart) {
            // If the track is set to restart, reset the note index to the beginning
            currentNote = 0;
        } else {
            // Otherwise, switch back to the previously active track
            set_current_track(get_previous_track());
        }
    }
}
