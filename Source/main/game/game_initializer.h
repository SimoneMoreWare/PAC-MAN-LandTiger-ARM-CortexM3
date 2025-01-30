#ifndef __GAME_INITIALIZER_H
#define __GAME_INITIALIZER_H

#include "../../timer/timer.h"
#include "game_engine.h"
#include "game_controller.h"

// Initialize the Pac-Man game
void initialize_pacman_game();

// Initialize all game timers
void initialize_game_timers();
void initialize_ghost_movement_timer();
#endif /* __GAME_INITIALIZER_H */
