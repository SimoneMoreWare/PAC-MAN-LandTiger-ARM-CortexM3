#include "timer_service.h"

/**
 * @brief Handles the interrupt for Timer 0 to process various game mechanics.
 * 
 * This timer is responsible for:
 * - Updating the remaining game time.
 * - Managing Blinky's respawn and vulnerability state.
 * - Generating special pills.
 * - Handling Pacman movement.
 */
void timer_0_next_tick_interrupt(void) {
    game_process_next_tick();  // Process the next game tick (includes Pacman movement, time, etc.)
}

/**
 * @brief Handles the interrupt for Timer 1 to manage Blinky's movement.
 * 
 * This timer is dedicated to controlling Blinky's movement logic
 * and processing Blinky-specific behaviors.
 */
void timer_1_move_ghost_interrupt(void) {
    game_process_movement_of_ghost();  // Process the movement of Blinky
}

/**
 * @brief Handles the interrupt for Timer 2 to play sound effects.
 * 
 * This timer is responsible for triggering and controlling 
 * sound effects in the game, such as playing a single note.
 */
void timer_2_play_tone_interrupt(void) {
    game_play_tone_single_note();  // Play a single note sound
}

/**
 * @brief Handles the interrupt for Timer 3 to play sound with a specific duration.
 * 
 * This timer is responsible for managing the duration of sound effects
 * in the game, such as completing the playback of a tone.
 */
void timer_3_play_tone_duration_interrupt(void) {
    game_play_tone_single_note_duration();  // Play a single note with a specific duration
}
