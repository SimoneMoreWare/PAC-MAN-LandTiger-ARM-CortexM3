#include "game_melodies.h"

/**
 * Retrieves a pointer to the track corresponding to the given TrackID.
 * If the specified TrackID is not found, returns a pointer to the first track in the list.
 * 
 * @param trackID The ID of the track to retrieve.
 * @return A pointer to the corresponding TRACK structure.
 */
 
static TrackID currentTrackPlaying = TRACK_BACKGROUND_MUSIC; // Holds the ID of the currently playing track
static TrackID previousTrackPlaying = TRACK_BACKGROUND_MUSIC; // Holds the ID of the previously playing track

#define DIMENSION_BACKGROUND_MUSIC 31
const NOTE BACKGROUND_MUSIC[] = 
{
    // Pacman main theme
    {NOTE_B4, time_16th }, {NOTE_B5, time_16th }, {NOTE_FS5, time_16th }, {NOTE_DS5, time_16th },  //1
    {NOTE_B5, time_16th_dotted}, {NOTE_FS5, time_8th_dotted}, {NOTE_DS5, time_8th}, {NOTE_C5, time_16th },
    {NOTE_C6, time_16th }, {NOTE_G6, time_16th }, {NOTE_E6, time_16th }, {NOTE_C6, time_16th_dotted}, {NOTE_G6, time_8th_dotted}, {NOTE_E6, time_8th},
    
    {NOTE_B4, time_16th }, {NOTE_B5, time_16th }, {NOTE_FS5, time_16th }, {NOTE_DS5, time_16th }, {NOTE_B5, time_16th_dotted},  //2
    {NOTE_FS5, time_8th_dotted}, {NOTE_DS5, time_8th}, {NOTE_DS5, time_16th_dotted}, {NOTE_E5, time_16th_dotted}, {NOTE_F5, time_16th_dotted},
    {NOTE_F5, time_16th_dotted}, {NOTE_FS5, time_16th_dotted}, {NOTE_G5, time_16th_dotted}, {NOTE_G5, time_16th_dotted}, {NOTE_GS5, time_16th_dotted}, {NOTE_A5, time_16th }, {NOTE_B5, time_8th}
};

#define DIMENSION_EAT_SPECIAL_PILL_SOUND_EFFECT 13
const NOTE EAT_SPECIAL_PILL_SOUND_EFFECT[] = {
    {NOTE_C4, time_16th},
    {NOTE_E4, time_16th},
    {NOTE_C4, time_16th},
    {REST, 		time_16th},
    {NOTE_C4, time_16th},
    {NOTE_E4, time_16th},
    {NOTE_C4, time_16th},
    {REST, 		time_16th},
    {NOTE_C4, time_16th},
    {NOTE_E4, time_16th},
    {NOTE_C4, time_16th},
    {REST, 		time_16th},
    {REST, 		time_4th},
};

#define DIMENSION_EAT_GHOST_SOUND_EFFECT 15
const NOTE EAT_GHOST_SOUND_EFFECT[] = {
    {NOTE_E4, time_4th},
    {NOTE_D4, time_4th},
    {NOTE_C4, time_4th},
    {NOTE_B3, time_4th},
    {NOTE_A3, time_4th},
    {NOTE_G3, time_4th},
    {REST, 		time_4th},
    {NOTE_E4, time_4th},
    {NOTE_D4, time_4th},
    {NOTE_C4, time_4th},
    {NOTE_B3, time_4th},
    {NOTE_A3, time_4th},
    {NOTE_G3, time_4th},
    {REST, 		time_4th},
    {REST, 		time_whole},
};

#define DIMENSION_LOSE_LIFE_SOUND_EFFECT 11
const NOTE LOSE_LIFE_SOUND_EFFECT[] = {
    {NOTE_C3, time_4th},
    {NOTE_G2, time_4th},
    {NOTE_E2, time_4th},
    {NOTE_C2, time_whole},
    {REST,	  time_16th},
    {NOTE_C3, time_4th},
    {NOTE_G2, time_4th},
    {NOTE_E2, time_4th},
    {NOTE_C2, time_whole},
    {REST, 		time_16th},
    {REST, 		time_whole},
};

#define DIMENSION_GAME_OVER_SOUND_EFFECT 11
const NOTE GAME_OVER_SOUND_EFFECT[] = {
    {NOTE_C3, time_whole},
    {NOTE_G2, time_whole},
    {NOTE_E2, time_4th},
    {REST, 		time_4th},
    {NOTE_C2, time_whole},
    {NOTE_C3, time_whole},
    {NOTE_G2, time_whole},
    {NOTE_E2, time_4th},
    {REST, 		time_4th},
    {NOTE_C2, time_whole},
    {REST, 		time_whole},
};

#define DIMENSION_EARN_NEW_LIFE_SOUND_EFFECT 6
const NOTE EARN_NEW_LIFE_SOUND_EFFECT[] = {
		{NOTE_E5, time_16th}, 
		{NOTE_G5, time_16th}, 
		{NOTE_E6, time_16th}, 
		{NOTE_C6, time_16th}, 
		{NOTE_D6, time_16th}, 
		{NOTE_G6, time_16th}
};

const unsigned char TRACKS_LIST_DIMENSION = 6;
const static TRACK TRACKS_LIST[] = {
	{
		.id 				= TRACK_BACKGROUND_MUSIC,
		.notes 			= BACKGROUND_MUSIC,
		.dimArray		= DIMENSION_BACKGROUND_MUSIC,
		.toRestart	= 1
	},
	{
		.id 				= TRACK_EAT_SPECIAL_PILL_SOUND_EFFECT,
		.notes 			= EAT_SPECIAL_PILL_SOUND_EFFECT,
		.dimArray		= DIMENSION_EAT_SPECIAL_PILL_SOUND_EFFECT,
		.toRestart	= 0
	},
	{
		.id 				= TRACK_EAT_GHOST_SOUND_EFFECT,
		.notes 			= EAT_GHOST_SOUND_EFFECT,
		.dimArray		= DIMENSION_EAT_GHOST_SOUND_EFFECT,
		.toRestart	= 0
	},
	{
		.id 				= TRACK_LOSE_LIFE_SOUND_EFFECT,
		.notes 			= LOSE_LIFE_SOUND_EFFECT,
		.dimArray		= DIMENSION_EAT_SPECIAL_PILL_SOUND_EFFECT,
		.toRestart	= 0
	},
	{
		.id 				= TRACK_GAME_OVER_SOUND_EFFECT,
		.notes 			= GAME_OVER_SOUND_EFFECT,
		.dimArray		= DIMENSION_GAME_OVER_SOUND_EFFECT,
		.toRestart	= 1
	},
	{
		.id 				= TRACK_EARN_NEW_LIFE_SOUND_EFFECT,
		.notes 			= EARN_NEW_LIFE_SOUND_EFFECT,
		.dimArray		= DIMENSION_EARN_NEW_LIFE_SOUND_EFFECT,
		.toRestart	= 0
	},
};

const TRACK* get_track_by_ID(TrackID trackID) {
    unsigned char i;
    for (i = 0; i < TRACKS_LIST_DIMENSION; i++) {
        if (TRACKS_LIST[i].id == trackID) {
            return &TRACKS_LIST[i];  // Found the track, return its address
        }
    }
    return &TRACKS_LIST[0];  // Default to the first track if not found
}

/**
 * Sets the track to be played.
 * Resets the current note index to 0 to start the new track from the beginning.
 * 
 * @param trackID The ID of the track to set as currently playing.
 */
void set_current_track(TrackID trackID) {
    currentTrackPlaying = trackID;  // Update the current track ID
    currentNote = 0;          			// Reset the note index for the new track
}

TrackID get_current_track(){
		return currentTrackPlaying;
}

TrackID get_previous_track(){
		return previousTrackPlaying;
}