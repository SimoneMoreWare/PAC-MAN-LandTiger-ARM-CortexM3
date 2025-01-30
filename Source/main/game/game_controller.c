#include "game_controller.h"
#include <math.h> // Per la funzione sqrt()

#define PROBABILITY_THRESHOLD 65     // Threshold for special pill generation in percentage
#define TIMER_COUNT           4      // Total number of available timers
#define MAX_ANIMATION_FRAMES 3 // Maximum number of frames for Pac-Man's animation
#define NO_COLLISION 0         // Indicates no collision
#define COLLISION 1            // Indicates a collision
#define TIME_COUNT_FOR_GAME_UPDATE 10 //1 s
#define TIME_COUNT_FOR_SPECIAL_PILL_GENERATION 7 // 0.7 s
#define TIME_1_SECOND 10


void game_trigger_failure();
void game_trigger_game_over();
void game_draw_pause_screen();
void game_pacman_spawn();
void game_ghost_spawn();
void pacman_update_position(Pacman* pacman, Position nextPosition, unsigned char* currentCell, unsigned char* nextCell, unsigned char isCollision);
void pacman_update_animation();
Position pacman_calculate_next_position(Pacman pacman);
void handle_pacman_movement(GameRunning* gameRunning, Position nextPosition); 
unsigned char game_handle_teleport(Position* position, Direction direction);
Position ghost_calculate_next_position(Ghost ghost);
void ghost_update_animation();
void ghost_update_position(Ghost* ghost, Position nextPosition, unsigned char* currentCell, unsigned char* nextCell, unsigned char isCollision);
Direction compute_ghost_direction(Ghost *ghost, Pacman *pacman);
unsigned char position_between_ghost_and_pacman_is_equal(Position posPacman, Position posGhost);
void process_pacman_ghost_collision();
void process_ghost_death();
void revive_ghost();
void score_update(CellType pill_type);
void lives_update();
int random_generate_seed();
Position random_generate_position();
void pill_transform_to_special(Position position);
void timer_toggle_set(unsigned char timer0, unsigned char timer1, unsigned char timer2, unsigned char timer3);
void timer_toggle_all();
unsigned char euclidean_distance(Position p1, Position p2);
void update_ghost_timer(unsigned char speed);
unsigned short calculate_timer_interval(unsigned char speed);
void update_game_time_display(unsigned char time_value);
void update_game_score_display(unsigned short score_value);
void update_lives_display(unsigned char lives_value);
void transmit_CAN_data();

static unsigned char ghostVunerableTickReset = 0;
static unsigned char isInHouse = 1;

void game_pacman_spawn(){
	gameRunning.pacman.sprite.direction = DIRECTION_STILL;
	gameRunning.pacman.sprite.prevPos = gameRunning.pacman.sprite.currPos;
	gameRunning.pacman.sprite.currPos = get_pacman_spawn_position();
}

void game_ghost_spawn(){
	gameRunning.ghost.sprite.direction = DIRECTION_STILL;
	gameRunning.ghost.sprite.prevPos = gameRunning.ghost.sprite.currPos;
	gameRunning.ghost.sprite.currPos = get_ghost_spawn_position();
}

// Initialize a level
void level_initialize(unsigned short is_first_initialization) {
    gameStatus.specialPillsGenerated = 0;
    gameRunning.remainingPills = GAME_CONFIG.standardPillCount;
    gameRunning.remainingTime = GAME_CONFIG.maxTime;

    initialize_game_map(&gameRunning.gameMap);
	
		game_pacman_spawn();
	
		game_ghost_spawn();

    if (is_first_initialization) {
        draw_game_map();
        draw_area_stat();
        game_draw_pause_screen();
    } else {
        update_game_time_display(gameRunning.remainingTime);
    }
}

void game_play_tone_all_track(){
		play_tone_all_track();
}

void game_play_tone_single_note(){
		play_tone_single_note();
}

void game_play_tone_single_note_duration(){
		play_tone_single_note_duration();
}

// Update the remaining game time
void game_update_time() {
    gameRunning.remainingTime--;
    update_game_time_display(gameRunning.remainingTime);

    if (gameRunning.remainingTime == 0) {
        game_trigger_failure();
    }
}

// Handle a game failure
void game_trigger_failure() {
    if (gameRunning.remainingLives == 0) {
        game_trigger_game_over();
        return;
    }
		
		set_current_track(TRACK_LOSE_LIFE_SOUND_EFFECT);

    timer_toggle_set(1, !gameStatus.ghostStatus.isDead, 0, 0);
	
    gameStatus.isFailed = 1;
    draw_cell(CELL_FREE, gameRunning.pacman.sprite.currPos);
		draw_cell(CELL_FREE, gameRunning.ghost.sprite.currPos);

    draw_screen_failure();

    gameRunning.remainingLives--;
		isInHouse = 1;
    lives_update();
    level_initialize(0);
}

// Handle a game over
void game_trigger_game_over() {
		set_current_track(TRACK_GAME_OVER_SOUND_EFFECT);
    gameStatus.isEnded = 1;
    timer_toggle_all();
    draw_screen_game_over();
}

// Handle a victory
void game_handle_victory() {
    if (gameRunning.remainingPills == 0) {
        gameStatus.isEnded = 1;
        timer_toggle_all();
        draw_screen_victory();
    }
}

// Handle game pause
void game_handle_pause() {
    if (gameStatus.isEnded) {
        return;
    }

    timer_toggle_all();

    if (gameStatus.isFailed) {
        gameStatus.isFailed = 0;
        undraw_pause_screen();
        draw_game_map();
        return;
    }
		draw_cell(CELL_FREE, gameRunning.pacman.sprite.currPos);
		draw_cell(CELL_FREE, gameRunning.ghost.sprite.currPos);
    gameStatus.isPaused = !gameStatus.isPaused;
    game_draw_pause_screen();
}

// Draw the pause screen
void game_draw_pause_screen() {
    if (gameStatus.isPaused) {
        draw_pause_screen();
    } else {
        undraw_pause_screen();
        redraw_after_pause();
    }
}

// Process the next game tick
void game_process_next_tick() {
    static unsigned short tickCountUpdateTime = 0;          // Global tick counter
    static unsigned short tickCountSpecialPillGeneration = 0; // Global tick counter
    static unsigned char blinkyRespawnTickCount = 0;        // Counter for Blinky's respawn
    static unsigned char blinkyBlueTickCount = 0;           // Counter for Blinky's vulnerability
		static unsigned char tickCountUpdateSpeedGhost = 0;
    tickCountUpdateTime++;	
    tickCountSpecialPillGeneration++;
		tickCountUpdateSpeedGhost++;
	
    if (ghostVunerableTickReset) {
        ghostVunerableTickReset = 0;
        blinkyBlueTickCount = 0;
    }

    // Handle Blinky's vulnerability state
    if (gameRunning.ghost.status.isVulnerable) {
        blinkyBlueTickCount++;
        if (blinkyBlueTickCount >= TIME_1_SECOND * GAME_CONFIG.ghostConfig.blueTime) {
            blinkyBlueTickCount = 0;
            gameRunning.ghost.status.isVulnerable = 0; // Blinky returns to normal
        }
    }

    // Handle Blinky's respawn state
    if (gameRunning.ghost.status.isDead) {
        blinkyRespawnTickCount++;
        if (blinkyRespawnTickCount >= TIME_1_SECOND * GAME_CONFIG.ghostConfig.respawnTime) {
            blinkyRespawnTickCount = 0;
            revive_ghost();
        }
    }
		// Calculate Pac-Man's next position based on its current direction
		Position nextPosition = pacman_calculate_next_position(gameRunning.pacman);
		
		// Handle Pac-Man's movement and interactions
    handle_pacman_movement(&gameRunning, nextPosition);
		
		if(tickCountUpdateTime >= TIME_COUNT_FOR_GAME_UPDATE){
			tickCountUpdateTime = 0;
			game_update_time();
		}

    // Generate special pills if needed
    if (tickCountSpecialPillGeneration >= TIME_COUNT_FOR_SPECIAL_PILL_GENERATION) {
        tickCountSpecialPillGeneration = 0;
        pill_generate_special();
    }
		
    // Handle Blinky's acceleration over time
		if ((tickCountUpdateSpeedGhost >= TIME_1_SECOND * gameRunning.ghost.config.accelInterval)) {
        tickCountUpdateSpeedGhost = 0;

        if (gameRunning.ghost.status.currentSpeed < GAME_CONFIG.ghostConfig.maxSpeed) {
            gameRunning.ghost.status.currentSpeed++; // Incrementa la velocità di Blinky
            update_ghost_timer(gameRunning.ghost.status.currentSpeed); // Aggiorna il timer
        }
    }
}

// Funzione per aggiornare il timer del fantasma in base alla velocità
void update_ghost_timer(unsigned char speed) {
    unsigned short interval = calculate_timer_interval(speed);

    // Disabilita e reimposta il timer
    disable_timer(1);
    reset_timer(1);

    // Inizializza il timer con il nuovo intervallo
    init_timer(1,0,0,3, interval);
		

    // Riabilita il timer
		//if(!gameRunning.ghost.status.isDead) enable_timer(1);
}

// Funzione per calcolare l'intervallo del timer in base alla velocità
unsigned short calculate_timer_interval(unsigned char speed) {
    // Parametri del timer del simulatore
    unsigned int baseInterval = 0x002625A0; // Intervallo per 0.1 secondi

    // Estremi di velocità e intervallo
    unsigned char maxSpeed = GAME_CONFIG.ghostConfig.maxSpeed;
    unsigned char minSpeed = GAME_CONFIG.ghostConfig.minSpeed;

    // Velocità minima corrisponde al massimo intervallo
    // Velocità massima corrisponde al minimo intervallo
    int intervalFactor = (maxSpeed - speed) / (maxSpeed - minSpeed);
		
    // Genera il nuovo intervallo
    return (unsigned int)(baseInterval * intervalFactor);
}


// Handle Pac-Man's movement and interactions with the game map
void handle_pacman_movement(GameRunning* gameRunning, Position nextPosition) {
    // Pointers to the current and next cells in the game map
    unsigned char* currentCell = &gameRunning->gameMap[gameRunning->pacman.sprite.currPos.y][gameRunning->pacman.sprite.currPos.x];
    unsigned char* nextCell = &gameRunning->gameMap[nextPosition.y][nextPosition.x];

    // Handle behavior based on the type of the next cell
    switch (*nextCell) {
        case CELL_FREE:
					  pacman_update_position(&gameRunning->pacman, nextPosition, currentCell, nextCell, NO_COLLISION);
						break;
        case CELL_TELEPORT_LEFT:
						pacman_update_position(&gameRunning->pacman, nextPosition, currentCell, nextCell, NO_COLLISION);
						break;
        case CELL_TELEPORT_RIGHT:
            // No collision, update Pac-Man's position
            pacman_update_position(&gameRunning->pacman, nextPosition, currentCell, nextCell, NO_COLLISION);
            break;
        case CELL_WALL:
            // Collision, keep Pac-Man in its current position
            pacman_update_position(&gameRunning->pacman, nextPosition, currentCell, nextCell, COLLISION);
            break;
				case CELL_GHOST_DOOR:
            // Collision, keep Pac-Man in its current position
						pacman_update_position(&gameRunning->pacman, nextPosition, currentCell, nextCell, COLLISION);
            break;
        case CELL_STANDARD_PILL:
						pacman_update_position(&gameRunning->pacman, nextPosition, currentCell, nextCell, NO_COLLISION);
            score_update(*nextCell); // Update the score based on the pill type
            lives_update();           // Check if an extra life should be awarded
            game_handle_victory();    // Check if all pills have been eaten
            *nextCell = CELL_FREE;   // Remove the pill from the map
            break;
        case CELL_SPECIAL_PILL:
            // No collision, Pac-Man eats the pill
            pacman_update_position(&gameRunning->pacman, nextPosition, currentCell, nextCell, NO_COLLISION);
            score_update(*nextCell); // Update the score based on the pill type
            lives_update();           // Check if an extra life should be awarded
            game_handle_victory();    // Check if all pills have been eaten
            *nextCell = CELL_FREE;   // Remove the pill from the map
            break;
    }
		
		if(position_between_ghost_and_pacman_is_equal(gameRunning->pacman.sprite.currPos, gameRunning->ghost.sprite.currPos) && !gameStatus.ghostStatus.isDead) {
						process_pacman_ghost_collision();
		}
}


// Update Pac-Man's position
void pacman_update_position(Pacman* pacman, Position nextPosition, unsigned char* currentCell, unsigned char* nextCell, unsigned char isCollision) {
    // Update the previous position
    pacman->sprite.prevPos = pacman->sprite.currPos;  // Accedi alla prevPos di Sprite

    // If there's no collision, update Pac-Man's current position
    if (isCollision == NO_COLLISION) {
        pacman->sprite.currPos = nextPosition;  // Accedi alla currPos di Sprite
    }

    // Update Pac-Man's animation
    pacman_update_animation();

    // Redraw the cells on the game map
    draw_cell(*currentCell, pacman->sprite.prevPos);  // Restore the previous cell
    draw_cell(CELL_PACMAN, pacman->sprite.currPos);  // Draw Pac-Man in the new position
}

// Update Pac-Man's animation frame
void pacman_update_animation() {
    // Increment the animation frame and reset if it exceeds the maximum
    if (++gameRunning.pacman.sprite.animationFrame > MAX_ANIMATION_FRAMES) {
        gameRunning.pacman.sprite.animationFrame = 0;
    }
}

// Calculate the next position for Pac-Man
Position pacman_calculate_next_position(Pacman pacman) {
    Position nextPosition = pacman.sprite.currPos;

    switch (pacman.sprite.direction) {
        case DIRECTION_STILL:
            break;
        case DIRECTION_RIGHT:
            if (game_handle_teleport(&nextPosition, pacman.sprite.direction)) break;
            nextPosition.x++;
            break;
        case DIRECTION_LEFT:
            if (game_handle_teleport(&nextPosition, pacman.sprite.direction)) break;
            nextPosition.x--;
            break;
        case DIRECTION_UP:
            nextPosition.y--;
            break;
        case DIRECTION_DOWN:
            nextPosition.y++;
            break;
    }
    return nextPosition;
}

void game_pacman_handle_direction_change(Direction newDirection) {
	gameRunning.pacman.sprite.direction = newDirection;
}

// Handle teleportation
unsigned char game_handle_teleport(Position* position, Direction direction) {
    unsigned char* cell = &gameRunning.gameMap[position->y][position->x];

    if ((*cell == CELL_TELEPORT_LEFT) && (direction == DIRECTION_LEFT)) {
        position->x = GAME_MAP_WIDTH - 1;
        return 1;
    }
    if ((*cell == CELL_TELEPORT_RIGHT) && (direction == DIRECTION_RIGHT)) {
        position->x = 0;
        return 1;
    }

    return 0;
}

// Handle ghost movement and interactions with the game map
void game_process_movement_of_ghost() {
    // Pointers to the current and next cells in the game map
		gameRunning.ghost.sprite.direction = compute_ghost_direction(&gameRunning.ghost, &gameRunning.pacman);
		Position nextPosition = ghost_calculate_next_position(gameRunning.ghost);
    unsigned char* currentCell = &gameRunning.gameMap[gameRunning.ghost.sprite.currPos.y][gameRunning.ghost.sprite.currPos.x];
    unsigned char* nextCell = &gameRunning.gameMap[nextPosition.y][nextPosition.x];

     // Handle behavior based on the type of the next cell
    switch (*nextCell) {
        case CELL_FREE:
						ghost_update_position(&gameRunning.ghost, nextPosition, currentCell, nextCell, NO_COLLISION);
						break;
        case CELL_TELEPORT_LEFT:
					  ghost_update_position(&gameRunning.ghost, nextPosition, currentCell, nextCell, NO_COLLISION);
						break;
        case CELL_TELEPORT_RIGHT:
            // No collision, update ghost position
            ghost_update_position(&gameRunning.ghost, nextPosition, currentCell, nextCell, NO_COLLISION);
            break;
        case CELL_WALL:
            // Collision, keep ghost- in its current position
            ghost_update_position(&gameRunning.ghost, nextPosition, currentCell, nextCell, COLLISION);
            break;
				case CELL_GHOST_DOOR:
            // Collision, keep ghost in its current position
            ghost_update_position(&gameRunning.ghost, nextPosition, currentCell, nextCell, !isInHouse);
            break;
        case CELL_STANDARD_PILL:
					  ghost_update_position(&gameRunning.ghost, nextPosition, currentCell, nextCell, NO_COLLISION);
						break;
        case CELL_SPECIAL_PILL:
            ghost_update_position(&gameRunning.ghost, nextPosition, currentCell, nextCell, NO_COLLISION);
						break;
				
		}	
		
		if(position_between_ghost_and_pacman_is_equal(gameRunning.pacman.sprite.currPos, gameRunning.ghost.sprite.currPos) && !gameStatus.ghostStatus.isDead) {
						process_pacman_ghost_collision();
				}
}
		
// Calcola la direzione di movimento per il fantasma rosso (Blinky)
Direction compute_ghost_direction(Ghost *ghost, Pacman *pacman) {
    // Direzione iniziale di default
    Direction chosenDirection = ghost->sprite.direction;
    unsigned char index;

    // Flag per controllare se il fantasma è ancora nella casa

    // Sequenza iniziale per uscire dalla casa
    if (isInHouse) {
        if (gameRunning.gameMap[ghost->sprite.currPos.y][ghost->sprite.currPos.x] == CELL_GHOST_DOOR) {
            // Se il fantasma è già sulla porta, si muove in basso per uscire
            chosenDirection = DIRECTION_UP;
            isInHouse = 0; // Il fantasma ha lasciato la casa
        } else {
            // Muovi il fantasma verso la porta (esempio: verso destra)
            chosenDirection = DIRECTION_UP;
        }

        // Aggiorna la direzione del fantasma e restituisci
        ghost->sprite.direction = chosenDirection;
        return chosenDirection;
    }

    // Posizioni candidate per il movimento (destra, sinistra, su, giù)
    const Position possibleMoves[4] = {
        {.x = ghost->sprite.currPos.x + 1, .y = ghost->sprite.currPos.y},  // DESTRA
        {.x = ghost->sprite.currPos.x - 1, .y = ghost->sprite.currPos.y},  // SINISTRA
        {.x = ghost->sprite.currPos.x, .y = ghost->sprite.currPos.y - 1},  // SU
        {.x = ghost->sprite.currPos.x, .y = ghost->sprite.currPos.y + 1},  // GIÙ
    };
		
		const Direction possibleDirections[4] = {
        DIRECTION_RIGHT, DIRECTION_LEFT, DIRECTION_UP, DIRECTION_DOWN
    };
		
		// Determina la direzione opposta a quella attuale
    Direction oppositeDirection;
    switch (ghost->sprite.direction) {
        case DIRECTION_RIGHT: oppositeDirection = DIRECTION_LEFT; break;
        case DIRECTION_LEFT: oppositeDirection = DIRECTION_RIGHT; break;
        case DIRECTION_UP: oppositeDirection = DIRECTION_DOWN; break;
        case DIRECTION_DOWN: oppositeDirection = DIRECTION_UP; break;
        default: oppositeDirection = ghost->sprite.direction; break;
    }


    unsigned char maxDistance = 0;
    unsigned char minDistance = 255;

    // Variabili temporanee per il calcolo
    Position tempPosition;
    unsigned char tempDistance;

    // Ciclo attraverso i movimenti possibili
    for (index = 0; index < 4; index++) {
			
				// Esclude la direzione opposta a quella attuale
        if (possibleDirections[index] == oppositeDirection) {
            continue;
        }

				
        tempPosition = possibleMoves[index];
			
				

        // Esclude posizioni non valide (muri o posizioni precedenti)
        if (gameRunning.gameMap[tempPosition.y][tempPosition.x] == CELL_WALL || 
						gameRunning.gameMap[tempPosition.y][tempPosition.x] == CELL_GHOST_DOOR || 
						(tempPosition.x == ghost->sprite.prevPos.x && tempPosition.y == ghost->sprite.prevPos.y)) {
            continue;
        }

        // Calcola la distanza ibrida tra la posizione corrente e quella di Pacman
        tempDistance = euclidean_distance(tempPosition, pacman->sprite.currPos);

        if (ghost->status.isVulnerable) {  // Modalità spaventata
            if (tempDistance > maxDistance) {
                maxDistance = tempDistance;
                chosenDirection = (Direction)(index + 1);
            }
        } else {  // Modalità di inseguimento
            if (tempDistance < minDistance) {
                minDistance = tempDistance;
                chosenDirection = (Direction)(index + 1);
            }
        }
    }

    // Aggiorna la direzione del fantasma
    ghost->sprite.direction = chosenDirection;
    return chosenDirection;
}

// Calcola la distanza Euclidea tra due posizioni
unsigned char euclidean_distance(Position p1, Position p2) {
    short dx = (short)p1.x - (short)p2.x;
    short dy = (short)p1.y - (short)p2.y;
    return (unsigned char)sqrtf((dx * dx) + (dy * dy));
}
		
// Update Pac-Man's position
void ghost_update_position(Ghost* ghost, Position nextPosition, unsigned char* currentCell, unsigned char* nextCell, unsigned char isCollision) {
    // Update the previous position
    ghost->sprite.prevPos = ghost->sprite.currPos;  // Accedi alla prevPos di Sprite

    // If there's no collision, update Pac-Man's current position
    if (isCollision == NO_COLLISION) {
        ghost->sprite.currPos = nextPosition;  // Accedi alla currPos di Sprite
    }

    // Update Pac-Man's animation
    ghost_update_animation();

    // Redraw the cells on the game map
    draw_cell(*currentCell, ghost->sprite.prevPos);  // Restore the previous cell
    draw_cell(CELL_GHOST, ghost->sprite.currPos);  // Draw Pac-Man in the new position
}

void ghost_update_animation() {
    // Increment the animation frame and reset if it exceeds the maximum
    if (++gameRunning.ghost.sprite.animationFrame > MAX_ANIMATION_FRAMES) {
        gameRunning.ghost.sprite.animationFrame = 0;
    }
}

// Calculate the next position for Pac-Man
Position ghost_calculate_next_position(Ghost ghost) {
    Position nextPosition = ghost.sprite.currPos;

    switch (ghost.sprite.direction) {
        case DIRECTION_STILL:
            break;
        case DIRECTION_RIGHT:
            if (game_handle_teleport(&nextPosition, ghost.sprite.direction)) break;
            nextPosition.x++;
            break;
        case DIRECTION_LEFT:
            if (game_handle_teleport(&nextPosition, ghost.sprite.direction)) break;
            nextPosition.x--;
            break;
        case DIRECTION_UP:
            nextPosition.y--;
            break;
        case DIRECTION_DOWN:
            nextPosition.y++;
            break;
    }
    return nextPosition;
}

unsigned char position_between_ghost_and_pacman_is_equal(Position posPacman, Position posGhost) {
	return ((posPacman.x == posGhost.x) && (posPacman.y == posGhost.y));
}


// Process the collision between Pac-Man and a ghost
void process_pacman_ghost_collision() {
    if (gameRunning.ghost.status.isVulnerable) {
        process_ghost_death();
        score_update(GAME_CONFIG.ghostEatPoint);
    } else {
        game_trigger_failure();
    }
}

// Process the death of a ghost (Blinky)
void process_ghost_death() {
    gameRunning.ghost.status.isDead = 1;
    gameRunning.ghost.status.isVulnerable = 0;
		isInHouse = 1;
    ghostVunerableTickReset = 1;

    disable_timer(1);  // Disable the timer for the ghost's actions
}

// Revive the ghost (Blinky)
void revive_ghost() {
		game_ghost_spawn();
    gameRunning.ghost.status.currentSpeed = GAME_CONFIG.ghostConfig.minSpeed;
	  
		reset_timer(1);
		init_timer(1,0,0,3, 0x002625A0);
    enable_timer(1);  // Enable the timer for the ghost's actions
	  gameRunning.ghost.status.isDead = 0; // Blinky is revived

}

// Update the score based on the pill type
void score_update(CellType pill_type) {
    if (pill_type == CELL_STANDARD_PILL) {
        gameRunning.score += GAME_CONFIG.standardPillPoint;
        gameRunning.remainingPills--;
    } else if (pill_type == CELL_SPECIAL_PILL) {
				gameRunning.ghost.status.isVulnerable = 1;
				ghostVunerableTickReset = 1;
				set_current_track(TRACK_EAT_SPECIAL_PILL_SOUND_EFFECT);
        gameRunning.score += GAME_CONFIG.specialPillPoint;
        gameRunning.remainingPills--;
    }

    update_game_score_display(gameRunning.score);
}

// Update the number of lives
void lives_update() {
    static unsigned char life_increases = 0;
    if (gameRunning.score >= ((life_increases + 1) * GAME_CONFIG.pointsPerExtraLife)) {
        life_increases++;
        if (gameRunning.remainingLives < GAME_CONFIG.maxLives) {
            gameRunning.remainingLives++;
        }
    }
		set_current_track(TRACK_EARN_NEW_LIFE_SOUND_EFFECT);
    update_lives_display(gameRunning.remainingLives);
}

/**
 * @brief Updates the game time display based on the current environment.
 * 
 * If the program is running in the simulator, it directly updates the display
 * using the `draw_time_stat` function. Otherwise, it sends the updated game time
 * via CAN communication.
 * 
 * @param time_value The current game time to display.
 */
void update_game_time_display(unsigned char time_value) {
    if (IS_SIMULATOR) {  // No CAN communication in the simulator
        draw_time_stat(time_value);  // Update the time display directly
    } else {
        transmit_CAN_data();  // Send updated game data via CAN
    }
}

/**
 * @brief Updates the game score display based on the current environment.
 * 
 * If the program is running in the simulator, it directly updates the display
 * using the `draw_stat_score` function. Otherwise, it sends the updated game score
 * via CAN communication.
 * 
 * @param score_value The current game score to display.
 */
void update_game_score_display(unsigned short score_value) {
    if (IS_SIMULATOR) {  // No CAN communication in the simulator
        draw_stat_score(score_value);  // Update the score display directly
    } else {
        transmit_CAN_data();  // Send updated game data via CAN
    }
}

/**
 * @brief Updates the lives display based on the current environment.
 * 
 * If the program is running in the simulator, it directly updates the display
 * using the `draw_status_lives` function. Otherwise, it sends the updated lives count
 * via CAN communication.
 * 
 * @param lives_value The current number of lives to display.
 */
void update_lives_display(unsigned char lives_value) {
    if (IS_SIMULATOR) {  // No CAN communication in the simulator
        draw_status_lives(lives_value);  // Update the lives display directly
    } else {
        transmit_CAN_data();  // Send updated game data via CAN
    }
}

/**
 * @brief Transmits the current game data via CAN communication.
 * 
 * This function encodes the current game state (remaining time, lives, and score) 
 * into a CAN message and sends it using the `CAN_wrMsg` function.
 */
void transmit_CAN_data() {
    // Encode the remaining time, lives, and score into the CAN message
    CAN_TxMsg.data[0] = gameRunning.remainingTime;           // Remaining time
    CAN_TxMsg.data[1] = gameRunning.remainingLives;          // Remaining lives
    CAN_TxMsg.data[2] = (gameRunning.score & 0xFF00) >> 8;   // High byte of the score
    CAN_TxMsg.data[3] = gameRunning.score & 0xFF;            // Low byte of the score

    // Set CAN message properties
    CAN_TxMsg.len = 4;                  // Data length
    CAN_TxMsg.id = 2;                   // Message ID
    CAN_TxMsg.format = STANDARD_FORMAT; // Standard message format
    CAN_TxMsg.type = DATA_FRAME;        // Data frame type

    // Transmit the CAN message on channel 1
    CAN_wrMsg(1, &CAN_TxMsg);
}


// Generate a special pill
void pill_generate_special() {
    // Generate a random timer index and use its value for randomness
    int random_timer_index = rand() % TIMER_COUNT; // Select a random timer (0 to 3)
    unsigned char random_value = get_game_timer_value(random_timer_index) % 101; // Normalize to [0, 100]

    // Check if the random value exceeds the threshold or if we already generated enough special pills
    if ((random_value > PROBABILITY_THRESHOLD) || (gameStatus.specialPillsGenerated >= GAME_CONFIG.specialPillCount)) {
        return; // Do not generate a new special pill
    }

    // Generate a random position and check if the cell contains a standard pill
    Position random_position = random_generate_position();
    if (gameRunning.gameMap[random_position.y][random_position.x] == CELL_STANDARD_PILL) {
        gameRunning.gameMap[random_position.y][random_position.x] = CELL_SPECIAL_PILL; // Transform to special pill
        pill_transform_to_special(random_position);
        gameStatus.specialPillsGenerated++; // Increment the count of special pills generated
    }
}

// Transform a pill into a special pill by updating the map and drawing the cell
void pill_transform_to_special(Position position) {
    draw_cell(CELL_SPECIAL_PILL, position); // Draw the special pill on the game map
}

// Generate a random seed using a combination of two random timers and predefined constants
int random_generate_seed() {
    int random_timer_index1 = rand() % TIMER_COUNT; // Select the first random timer index
    int random_timer_index2 = rand() % TIMER_COUNT; // Select the second random timer index

    // Ensure the two indices are different
    while (random_timer_index1 == random_timer_index2) {
        random_timer_index2 = rand() % TIMER_COUNT;
    }

    // Combine the values of two timers with other constants to generate the seed
    return get_game_timer_value(random_timer_index1) ^ 
           get_game_timer_value(random_timer_index2) ^ 
           get_RIT_value() ^ 
           RANDOM_SEED_CONSTANT;
}

// Generate a random position within the game map
Position random_generate_position() {
    srand(random_generate_seed()); // Seed the random number generator
    return (Position){.y = rand() % GAME_MAP_HEIGHT, .x = rand() % GAME_MAP_WIDTH}; // Generate random coordinates
}

void timer_toggle_set(unsigned char timer0, unsigned char timer1, unsigned char timer2, unsigned char timer3) {
	if(timer0)
		start_stop_game_timer(0);
	
	if(timer1)
		start_stop_game_timer(1);
	
	if(timer2)
		start_stop_game_timer(2);
	
	if(timer3)
		start_stop_game_timer(3);
}

// Toggle all game timers
void timer_toggle_all() {
	  start_stop_game_timer(0);
		start_stop_game_timer(1);
		start_stop_game_timer(2);
		start_stop_game_timer(3);
}
