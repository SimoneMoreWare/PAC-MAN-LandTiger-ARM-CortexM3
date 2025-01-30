#include "game_initializer.h"

// Function prototypes (internal to this file)
void initialize_game_timer();
//void initialize_buzzer_timer();

// Initialize the Pac-Man game state
void initialize_pacman_game() {
    gameRunning.remainingLives = GAME_CONFIG.startLives; // Set starting lives
    gameRunning.score = 0;                         // Reset score
    gameStatus.isPaused = 1;                       // Start with the game paused
    gameStatus.ghostStatus.currentSpeed = GAME_CONFIG.ghostConfig.minSpeed;
    level_initialize(1); // Initialize level 1
}

// Setup all game timers
void initialize_game_timers() {
	
    initialize_game_timer();
    initialize_ghost_movement_timer();
	
    //initialize_special_pills_timer();
}

// Setup the game timer (1-second interval)
void initialize_game_timer() {
    //init_timer(0, 0, 0, 3, 0x017D7840); // 1 second timer
	  init_timer(0, 0, 0, 3, 0x002625A0); // 0.1 seconds timer for the simulator
}

void initialize_ghost_movement_timer(){
		 init_timer(1, 0, 0, 3, 0x002625A0); // 0.1 seconds timer for the simulator
}

/*
// Setup the Pac-Man movement timer
void initialize_buzzer_timer() {
    if (!IS_SIMULATOR) {
        init_timer(2, 0, 0, 3, 0x00319750); // 0.13 seconds timer for real hardware
    } else {
        init_timer(2, 0, 0, 3, 0x00225510); // 0.09 seconds timer for the simulator
    }
}

// Setup the special pills timer (0.7-second interval)
void initialize_special_pills_timer() {
    init_timer(3, 0, 0, 3, 0x010B0760); // 0.7 seconds timer
}
*/