#include "game_canComuntication.h"

// Static variables to store the previous state of game stats
static unsigned char previous_time;   // Previous game time
static unsigned short previous_score; // Previous game score
static unsigned char previous_lives;  // Previous number of lives

/**
 * @brief Handles CAN communication to update game statistics.
 * 
 * This function decodes the incoming data array, extracts the game statistics,
 * and updates the corresponding values if there are any changes.
 * 
 * @param data_array An array of 8 bytes containing the game statistics.
 *        - data_array[0]: Game time (unsigned char)
 *        - data_array[1]: Number of lives (unsigned char)
 *        - data_array[2] and data_array[3]: Game score (unsigned short, high byte first)
 */
void handle_CAN_update_stats(unsigned char data_array[8]) {
    // Local variables to hold the decoded game statistics
    unsigned char current_time;   // Current game time
    unsigned short current_score; // Current game score
    unsigned char current_lives;  // Current number of lives
    
    // Decode the data array
    current_time = data_array[0];                       // Extract game time
    current_lives = data_array[1];                      // Extract number of lives
    current_score = (data_array[2] << 8) | data_array[3]; // Extract game score (big-endian)

    // Update game time if it has changed
    if (current_time != previous_time) {
        previous_time = current_time;  // Update previous time
        draw_time_stat(current_time);  // Call function to display updated time
    }

    // Update game score if it has changed
    if (current_score != previous_score) {
        previous_score = current_score;  // Update previous score
        draw_stat_score(current_score);  // Call function to display updated score
    }

    // Update number of lives if it has changed
    if (current_lives != previous_lives) {
        previous_lives = current_lives;  // Update previous lives
        draw_status_lives(current_lives);  // Call function to display updated lives
    }
}
