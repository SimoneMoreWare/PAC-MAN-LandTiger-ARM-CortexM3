/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Description:         Functions to initialize and configure the joystick
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "joystick.h"

/*----------------------------------------------------------------------------
  Function to initialize the joystick pins and configure them as inputs
 *----------------------------------------------------------------------------*/

void joystick_init(void) {
	
	/* Configure the UP direction of the joystick */
	LPC_PINCON->PINSEL3 &= ~(3<<26);	// Set P1.29 as GPIO (00b)
	LPC_GPIO1->FIODIR   &= ~(1<<29);	// Set P1.29 as input
	
	/* Configure the RIGHT direction of the joystick */
	LPC_PINCON->PINSEL3 &= ~(3<<24);	// Set P1.28 as GPIO (00b)
  LPC_GPIO1->FIODIR   &= ~(1<<28);	// Set P1.28 as input
	
	/* Configure the LEFT direction of the joystick */
	LPC_PINCON->PINSEL3 &= ~(3<<22);	// Set P1.27 as GPIO (00b)
  LPC_GPIO1->FIODIR   &= ~(1<<27);	// Set P1.27 as input 
	
	/* Configure the DOWN direction of the joystick */
	LPC_PINCON->PINSEL3 &= ~(3<<20);	// Set P1.26 as GPIO (00b)
	LPC_GPIO1->FIODIR   &= ~(1<<26);	// Set P1.26 as input 
	
}
