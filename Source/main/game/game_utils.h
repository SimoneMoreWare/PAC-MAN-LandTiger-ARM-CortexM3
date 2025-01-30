#ifndef __GAME_UTILS_H
#define __GAME_UTILS_H

#include <stdio.h>
#include <string.h>
#include "game_engine.h"

// Macro definitions
#define NUM_TIMERS 4

// Function prototypes
void initialize_game_map(GameMap* map_to_initialize);
Position get_pacman_spawn_position(void);
Position get_ghost_spawn_position(void);
void start_stop_game_timer(uint8_t timer_id);
unsigned int get_game_timer_value(uint8_t timer_id);
unsigned int get_RIT_value(void);

#endif /* __GAME_UTILS_H */
