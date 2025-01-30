#include "game_sound.h"

typedef enum {
	TRACK_BACKGROUND_MUSIC								= 0,
	TRACK_EAT_SPECIAL_PILL_SOUND_EFFECT		= 1,
	TRACK_EAT_GHOST_SOUND_EFFECT					= 2,
	TRACK_LOSE_LIFE_SOUND_EFFECT					= 3,
	TRACK_GAME_OVER_SOUND_EFFECT					= 4,
	TRACK_EARN_NEW_LIFE_SOUND_EFFECT 			= 5
} TrackID;

typedef struct {
	TrackID 			 id;					// ID of the track
	const    NOTE* notes;				// Composition
	unsigned char  dimArray;		// Array dimension
	unsigned char  toRestart;		// to play for ever?
} TRACK;

void set_track_to_play(TrackID trackID);
const TRACK* get_track_by_ID(TrackID trackID);
void set_current_track(TrackID trackID);
