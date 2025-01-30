/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2024-12-27
** Last Version:        V1.02
** Descriptions:        Functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "../main/rit_handler.h"

/* Variabili Globali Gestione De-Bouncing */
volatile int down_0 = 0;
volatile int Joystick_up = 0, Joystick_down = 0, Joystick_right = 0, Joystick_left = 0;

const int SHORT_PRESS_COUNT = 2;

/******************************************************************************
** Function name:		RIT_IRQHandler
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
******************************************************************************/
void RIT_IRQHandler(void) {			
    /* Gestione INT0 */
    if (down_0 != 0) {
        down_0++;
        if ((LPC_GPIO2->FIOPIN & (1 << 10)) == 0) { /* Button still pressed */
            switch (down_0) {
                case SHORT_PRESS_COUNT:
                    pause_game();
                    break;
                default:
                    break;
            }
        } else { /* Button released */
            down_0 = 0;
            NVIC_EnableIRQ(EINT0_IRQn);   /* Re-enable interrupt */
            LPC_PINCON->PINSEL4 |= (1 << 20);
        }
    }

    /* Gestione Joystick UP */
    if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0) { /* Joystick UP pressed */
        Joystick_up++;
        if (Joystick_up == 1) {
            update_direction_up();
        }
    } else {
        Joystick_up = 0;
    }

    /* Gestione Joystick DOWN */
    if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0) { /* Joystick DOWN pressed */
        Joystick_down++;
        if (Joystick_down == 1) {
            update_direction_down();
        }
    } else {
        Joystick_down = 0;
    }

    /* Gestione Joystick RIGHT */
    if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0) { /* Joystick RIGHT pressed */
        Joystick_right++;
        if (Joystick_right == 1) {
            update_direction_right();
        }
    } else {
        Joystick_right = 0;
    }

    /* Gestione Joystick LEFT */
    if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0) { /* Joystick LEFT pressed */
        Joystick_left++;
        if (Joystick_left == 1) {
            update_direction_left();
        }
    } else {
        Joystick_left = 0;
    }
		
		update_speaker_sound();

    reset_RIT();
    LPC_RIT->RICTRL |= 0x1; /* Clear interrupt flag */
}
