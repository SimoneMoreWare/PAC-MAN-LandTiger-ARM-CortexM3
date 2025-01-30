/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           lib_RIT.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        Atomic functions to be used by higher software levels
** Correlated files:    lib_RIT.c, funct_RIT.c, IRQ_RIT.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"

/******************************************************************************
** Function name:		enable_RIT
**
** Descriptions:		Enable Repetitive Interrupt Timer (RIT)
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void enable_RIT(void) {
    LPC_RIT->RICTRL |= (1 << 3);  // Set the enable bit for RIT
}

/******************************************************************************
** Function name:		disable_RIT
**
** Descriptions:		Disable Repetitive Interrupt Timer (RIT)
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void disable_RIT(void) {
    LPC_RIT->RICTRL &= ~(1 << 3);  // Clear the enable bit for RIT
}

/******************************************************************************
** Function name:		reset_RIT
**
** Descriptions:		Reset the Repetitive Interrupt Timer (RIT)
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void reset_RIT(void) {
    LPC_RIT->RICOUNTER = 0;  // Reset the RIT counter
}

/******************************************************************************
** Function name:		init_RIT
**
** Descriptions:		Initialize the Repetitive Interrupt Timer (RIT) with a specific interval
**
** parameters:			RITInterval - Interval value to set for RIT match
** Returned value:		0 on success
**
******************************************************************************/
uint32_t init_RIT(uint32_t RITInterval) {
    // Set up the RIT clock to use the main CCLK
    LPC_SC->PCLKSEL1 &= ~(3 << 26);   // Clear the previous clock setting
    LPC_SC->PCLKSEL1 |= (1 << 26);    // Set the RIT clock to CCLK
    LPC_SC->PCONP |= (1 << 16);       // Enable power to RIT module
    LPC_RIT->RICOMPVAL = RITInterval; // Set the RIT match value
    LPC_RIT->RICTRL = (1 << 1) |      // Enable RIT to clear on match
                      (1 << 2);       // Enable timer for debugging
    LPC_RIT->RICOUNTER = 0;           // Reset the RIT counter
    NVIC_EnableIRQ(RIT_IRQn);         // Enable the RIT interrupt
    return 0;                         // Return success
}

/******************************************************************************
**                            End Of File
******************************************************************************/  
