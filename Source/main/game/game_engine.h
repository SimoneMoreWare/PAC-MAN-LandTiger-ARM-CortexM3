#ifndef __PACMAN_GAME_CORE_H
#define __PACMAN_GAME_CORE_H

#include "LPC17xx.h"

#ifdef SIMULATOR
    #define IS_SIMULATOR 1
#else
    #define IS_SIMULATOR 0
#endif

/* Display Dimensions */
#define DISPLAY_HEIGHT       320
#define DISPLAY_WIDTH        240
#define TILE_SCALE           8
#define DISPLAY_CENTER_X     (DISPLAY_WIDTH / 2)
#define DISPLAY_CENTER_Y     (DISPLAY_HEIGHT / 2)

/* Game Map Dimensions */
#define GAME_MAP_HEIGHT      31
#define GAME_MAP_WIDTH       28
#define GAME_MAP_PIXEL_HEIGHT (GAME_MAP_HEIGHT * TILE_SCALE)
#define GAME_MAP_PIXEL_WIDTH  (GAME_MAP_WIDTH * TILE_SCALE)

/* Display Padding */
#define DISPLAY_PADDING_LEFT   ((DISPLAY_WIDTH - GAME_MAP_PIXEL_WIDTH) / 2)
#define DISPLAY_PADDING_RIGHT  DISPLAY_PADDING_LEFT
#define DISPLAY_PADDING_TOP    8
#define DISPLAY_PADDING_BOTTOM 24

#define GAME_MAP_START_X     (0 + DISPLAY_PADDING_LEFT)
#define GAME_MAP_START_Y     (DISPLAY_HEIGHT - (GAME_MAP_HEIGHT * TILE_SCALE) - DISPLAY_PADDING_BOTTOM)
#define GAME_MAP_END_Y       (GAME_MAP_START_Y + GAME_MAP_HEIGHT * TILE_SCALE)

/* GUI Constants */
#define PLAYER_GUI_DELIMITER  8
#define PLAYER_GUI_HEIGHT     (DISPLAY_HEIGHT - GAME_MAP_PIXEL_HEIGHT - DISPLAY_PADDING_TOP - DISPLAY_PADDING_BOTTOM - PLAYER_GUI_DELIMITER)
#define PLAYER_GUI_WIDTH      (GAME_MAP_PIXEL_WIDTH)
#define PLAYER_GUI_START_X    DISPLAY_PADDING_LEFT
#define PLAYER_GUI_START_Y    DISPLAY_PADDING_TOP

/* Timer and Score Display Areas */
#define TIMER_AREA_START_X    PLAYER_GUI_START_X
#define SCORE_AREA_START_X    (TIMER_AREA_START_X + (PLAYER_GUI_WIDTH / 2))

/* Player Info Layout */
#define TIMER_VALUE_START_Y   (PLAYER_GUI_START_Y + 20)
#define SCORE_VALUE_START_Y   TIMER_VALUE_START_Y
#define LIVES_VALUE_START_Y   (DISPLAY_HEIGHT - DISPLAY_PADDING_BOTTOM + 8)

#define TIMER_VALUE_START_X   (TIMER_AREA_START_X + 20)
#define SCORE_VALUE_START_X   (SCORE_AREA_START_X + 16)
#define LIVES_VALUE_START_X   (16)

/* Game Constants */
#define PACMAN_SPAWN_X        14
#define PACMAN_SPAWN_Y        23
#define GHOST_SPAWN_X 				13
#define GHOST_SPAWN_Y					12

/* Type Definitions */

// Direction Enum
typedef enum {
    DIRECTION_STILL  = 0, // No movement
    DIRECTION_RIGHT  = 1, // Right
    DIRECTION_LEFT   = 2, // Left
    DIRECTION_UP     = 3, // Up
    DIRECTION_DOWN   = 4  // Down
} Direction;

// Cell Types Enum (For the Game Map)
typedef enum {
    CELL_FREE        = 0,  // Empty Space
    CELL_WALL         = 1,  // Wall
    CELL_TELEPORT_LEFT      = 2,  // Teleport Area Left
    CELL_TELEPORT_RIGHT     = 3,  // Teleport Area Right
    CELL_STANDARD_PILL     = 4,  // Standard Pill
    CELL_SPECIAL_PILL     = 5,  // Special Pill
    CELL_PACMAN       = 6,  // PacMan
		CELL_GHOST_DOOR = 7, // Ghost door 
		CELL_GHOST 			= 8
} CellType;

// Game Map Definition
typedef uint8_t GameMap[GAME_MAP_HEIGHT][GAME_MAP_WIDTH];

// Position Structure (For Coordinates)
typedef struct {
    unsigned char x;
    unsigned char y;
} Position;

// Base Character Structure
typedef struct {
    Position currPos;       // Current position
    Position prevPos;       // Previous position
    Direction direction;    // Current movement direction
    unsigned char animationFrame; // Animation frame
} Sprite;

// PacMan Configuration
typedef struct {
    Sprite sprite;  // Base sprite (inherits common properties)
    unsigned char speed;  // PacMan's speed
} Pacman;

// Ghost Configuration
typedef struct {
    Sprite sprite;   // Base Character (inherits common properties)
    unsigned char minSpeed;    // Ghost's min speed
    unsigned char maxSpeed;    // Ghost's max speed
    unsigned short respawnTime; // Time to respawn
    unsigned char blueTime;     // Time when the ghost is vulnerable
    unsigned short accelInterval; // Interval at which the ghost accelerates
} GhostConfig;

// Ghost Status (State during game)
typedef struct {
    unsigned char currentSpeed;   // Current speed of the ghost
    unsigned char isDead;         // If the ghost is dead
    unsigned char isVulnerable;   // If the ghost is vulnerable
    unsigned short respawnTimer;  // Time remaining before respawn
    unsigned short vulnerabilityTimer;  // Vulnerability timer
} GhostStatus;

typedef struct {
	Sprite sprite;
	GhostConfig config;
  GhostStatus status;
} Ghost;

// Game Configuration Structure
typedef struct {
    unsigned char maxTime;           // Maximum time for the game
    unsigned char startLives;        // Number of lives at the start
    unsigned char maxLives;          // Maximum number of lives
    unsigned short pointsPerExtraLife; // Points for an extra life
    unsigned char standardPillCount;  // Number of standard pills
    unsigned char specialPillCount;   // Number of special pills
    unsigned short standardPillPoint; // Score for a standard pill
    unsigned short specialPillPoint;  // Score for a special pill
		unsigned short ghostEatPoint;		// Score Points when pacman eats a Ghost
    unsigned char pacmanSpeed;       // Pacman speed
    GhostConfig ghostConfig;         // Ghost configuration
} GameConfig;

// Game Running State
typedef struct {
    GameMap gameMap;               // Current game map
    unsigned char remainingTime;    // Time remaining
    unsigned char remainingLives;   // Remaining lives
    unsigned short score;           // Player's score
    unsigned char remainingPills;   // Number of remaining pills
    Pacman pacman;                  // PacMan's state
    Ghost ghost;                    // Blinky ghost state
} GameRunning;

// Game Status Structure
typedef struct {
    unsigned char isPaused;        // Game paused state
    unsigned char isFailed;        // Game failed state
    unsigned char isEnded;         // Game over or victory state
    unsigned char specialPillsGenerated; // Number of special pills generated
    GhostStatus ghostStatus;       // Ghost status during the game
} GameStatus;

extern const GameConfig GAME_CONFIG; // Global configuration
extern GameRunning gameRunning;      // Game running state
extern GameStatus gameStatus;       // Game status

#endif 