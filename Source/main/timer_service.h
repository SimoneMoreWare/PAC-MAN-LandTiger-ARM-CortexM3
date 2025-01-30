#ifndef __TIMER_SERVICE_H
#define __TIMER_SERVICE_H

#include "./game/game_controller.h"

// Timer Interrupt Handlers Header File

/*
 * TIMER0: 
 * - Handles time management for the game.
 * - Responsible for generating special pills and controlling Pac-Man's movement.
 */
void timer_0_next_tick_interrupt(void);

/*
 * TIMER1: 
 * - Reserved for Blinky's movement control.
 * (Currently disabled, uncomment the line below to use.)
 */
void timer_1_move_ghost_interrupt(void);

/*
 * TIMER2: 
 * - Controls sound generation by producing specific tones based on the game's state.
 */
void timer_2_play_tone_interrupt(void);

/*
 * TIMER3: 
 * - Manages the duration of sound playback.
 * - Ensures tones are played for the correct length of time.
 */
void timer_3_play_tone_duration_interrupt(void);

#endif /* End of TIMER INTERRUPTS HEADER */
