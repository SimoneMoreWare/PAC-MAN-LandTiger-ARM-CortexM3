#ifndef __GAME_CONTROLLER_H
#define __GAME_CONTROLLER_H

#include <stdlib.h>
#include "game_engine.h"
#include "game_renderer.h"
#include "game_utils.h"
#include "../game_melodies.h"
#include "../CAN/CAN.h"
// Constant for generating random seeds
#define RANDOM_SEED_CONSTANT 1103515245

// Level initialization
void level_initialize(unsigned short is_first_initialization);

// Game control functions
void game_play_tone_single_note();
void game_play_tone_single_note_duration();
void game_process_movement_of_ghost();
void game_handle_pause();
void game_update_time();
void game_process_next_tick();
void game_pacman_handle_direction_change(Direction new_direction);

// Pill-related functions
void pill_generate_special();

// Random utilities
int random_generate_seed();
Position random_generate_position();

#endif /* __GAME_CONTROLLER_H */
