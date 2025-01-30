#include "LPC17xx.h"
#include "adc.h"

/*----------------------------------------------------------------------------
  Function that initializes speaker
 *----------------------------------------------------------------------------*/
void speaker_init(void){
	LPC_PINCON -> PINSEL1 |= (1<<21);
	LPC_PINCON -> PINSEL1 &= ~(1<<20);
	LPC_GPIO0 -> FIODIR |= (1<<26);
}