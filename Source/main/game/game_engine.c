#include "game_engine.h"
// Global game configuration (initialized with default values)
const GameConfig GAME_CONFIG = {
    .maxTime = 60,                // Tempo massimo di gioco in secondi
    .startLives = 1,               // Numero di vite iniziali
    .maxLives = 5,                 // Numero massimo di vite
    .pointsPerExtraLife = 1000,    // Punti necessari per una vita extra
    .standardPillCount = 240,      // Numero di pillole standard
    .specialPillCount = 6,         // Numero di pillole speciali
    .standardPillPoint = 10,       // Punti per una pillola standard
    .specialPillPoint = 50,        // Punti per una pillola speciale
    .ghostEatPoint = 100,          // Punti quando Pacman mangia un fantasma
    .pacmanSpeed = 5,              // Velocità di Pacman
    .ghostConfig = {               // Configurazione del fantasma Blinky
        .minSpeed = 2,             // Velocità minima
        .maxSpeed = 6,             // Velocità massima
        .respawnTime = 3,          // Tempo di respawn (in secondi)
        .blueTime = 10,            // Durata vulnerabilità (in secondi)
        .accelInterval = 30        // Intervallo di accelerazione (in tick)
    }
};

// Game running state initialization
GameRunning gameRunning = {
    .gameMap = {},                 // Inizializza la mappa del gioco a vuota
    .remainingTime = 60,          // Tempo di gioco rimanente
    .remainingLives = 1,           // Numero iniziale di vite
    .score = 0,                    // Punteggio iniziale
    .remainingPills = 240,         // Totale pillole standard rimanenti
    .pacman = {                    // Stato iniziale di Pacman
        .sprite = {
            .currPos = {PACMAN_SPAWN_X, PACMAN_SPAWN_Y}, // Posizione iniziale di Pacman
            .prevPos = {PACMAN_SPAWN_X, PACMAN_SPAWN_Y},
            .direction = DIRECTION_STILL,
            .animationFrame = 0
        },
        .speed = 5                  // Velocità iniziale di Pacman
    },
    .ghost = {                     // Stato iniziale di Blinky (Fantasma)
        .sprite = {
            .currPos = {GHOST_SPAWN_X, GHOST_SPAWN_Y}, // Posizione iniziale del fantasma
            .prevPos = {GHOST_SPAWN_X, GHOST_SPAWN_Y},
            .direction = DIRECTION_STILL,
            .animationFrame = 0
        },
        .config = {                 // Configurazione del fantasma
            .minSpeed = 2,          // Velocità minima
            .maxSpeed = 6,          // Velocità massima
            .respawnTime = 3,       // Tempo di respawn
            .blueTime = 10,         // Tempo di vulnerabilità
            .accelInterval = 30     // Intervallo di accelerazione
        },
        .status = {                 // Stato iniziale del fantasma
            .currentSpeed = 2,      // Velocità iniziale
            .isDead = 0,            // Fantasma vivo
            .isVulnerable = 0,      // Fantasma non vulnerabile
            .respawnTimer = 0,      // Timer di respawn inattivo
            .vulnerabilityTimer = 0 // Timer di vulnerabilità inattivo
        }
    }
};

// Game status initialization
GameStatus gameStatus = {
    .isPaused = 0,                 // Game is not paused
    .isFailed = 0,                 // Game is not failed
    .isEnded = 0,                  // Game is not ended
    .specialPillsGenerated = 0,    // No special pills generated yet
    .ghostStatus = {               // Ghost status for Blinky
        .currentSpeed = 2,
        .isDead = 0,
        .isVulnerable = 0,
        .respawnTimer = 0,
        .vulnerabilityTimer = 0
    }
};
