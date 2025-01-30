#include "button.h"
#include "LPC17xx.h"

void button_init(void) {

	// Configure P2.10 as an external interrupt pin (EINT0) and set it as input
	LPC_PINCON->PINSEL4    |= (1 << 20);			// Assign functionality for external interrupt
	LPC_GPIO2->FIODIR      &= ~(1 << 10);    	// Set P2.10 as input
	
	// Set external interrupt mode for pins (falling edge or level-sensitive)
	LPC_SC->EXTMODE = 0x1;						// Enable external interrupt modes for EINT0, EINT1, EINT2

	// Enable interrupts for EINT0 and set its priority
	NVIC_EnableIRQ(EINT0_IRQn);						// Enable EINT0 interrupt in NVIC
	NVIC_SetPriority(EINT0_IRQn, 3);			// Set priority level 3 (lowest among the three)
}
