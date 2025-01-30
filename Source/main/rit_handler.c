#include "rit_handler.h"

#include "timer/timer.h"

// Aggiorna la direzione verso l'alto
void update_direction_up() {
    game_pacman_handle_direction_change(DIRECTION_UP);
}

// Aggiorna la direzione verso il basso
void update_direction_down() {
    game_pacman_handle_direction_change(DIRECTION_DOWN);
}

// Aggiorna la direzione verso destra
void update_direction_right() {
    game_pacman_handle_direction_change(DIRECTION_RIGHT);
}

// Aggiorna la direzione verso sinistra
void update_direction_left() {
    game_pacman_handle_direction_change(DIRECTION_LEFT);
}

// Mette in pausa il gioco
void pause_game() {
    game_handle_pause();
}

void update_speaker_sound(){
		game_play_tone_all_track();
}