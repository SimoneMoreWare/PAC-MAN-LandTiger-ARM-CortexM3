/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           lib_timer.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        atomic functions to be used by higher sw levels
** Correlated files:    lib_timer.c, funct_timer.c, IRQ_timer.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "timer.h"

/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		Enable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
**
******************************************************************************/
void enable_timer( uint8_t timer_id )
{
	if ( timer_id == 0 )
	{
		LPC_TIM0->TCR = 1;
	}
	else if (timer_id == 1)
	{
		LPC_TIM1->TCR = 1;
	}
	else if (timer_id == 2)
	{
		LPC_TIM2->TCR = 1;
	}
	else if (timer_id == 3)
	{
		LPC_TIM3->TCR = 1;
	}
	return;
}

/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		Disable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
**
******************************************************************************/
void disable_timer( uint8_t timer_id )
{
	if ( timer_id == 0 )
	{
		LPC_TIM0->TCR = 0;
	}
	else if (timer_id == 1)
	{
		LPC_TIM1->TCR = 0;
	}
	else if (timer_id == 2)
	{
		LPC_TIM2->TCR = 0;
	}
	else if (timer_id == 3)
	{
		LPC_TIM3->TCR = 0;
	}
	return;
}

/******************************************************************************
** Function name:		reset_timer
**
** Descriptions:		Reset timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
**
******************************************************************************/
void reset_timer( uint8_t timer_id )
{
	uint32_t regVal;

	if ( timer_id == 0 )
	{
		regVal = LPC_TIM0->TCR;
		regVal |= 0x02;
		LPC_TIM0->TCR = regVal;
	}
	else if (timer_id == 1)
	{
		regVal = LPC_TIM1->TCR;
		regVal |= 0x02;
		LPC_TIM1->TCR = regVal;
	}
	else if (timer_id == 2)
	{
		regVal = LPC_TIM2->TCR;
		regVal |= 0x02;
		LPC_TIM2->TCR = regVal;
	}
	else if (timer_id == 3)
	{
		regVal = LPC_TIM3->TCR;
		regVal |= 0x02;
		LPC_TIM3->TCR = regVal;
	}
	return;
}


/******************************************************************************
** Function name:		init_timer
**
** Descriptions:		Inits Timer
**
** parameters:
** Returned value:	
**
******************************************************************************/
uint32_t init_timer ( uint8_t timer_id, uint32_t Prescaler, uint8_t MatchReg, uint8_t SRImatchReg, uint32_t TimerInterval )
{
	if ( timer_id == 0 )
	{
		LPC_TIM0-> PR = Prescaler;
		
		if (MatchReg == 0){
			LPC_TIM0->MR0 = TimerInterval;
			LPC_TIM0->MCR |= SRImatchReg << 3*MatchReg;			
		}
		else if (MatchReg == 1){
			LPC_TIM0->MR1 = TimerInterval;
			LPC_TIM0->MCR |= SRImatchReg << 3*MatchReg;			
		}
		else if (MatchReg == 2){
			LPC_TIM0->MR2 = TimerInterval;
			LPC_TIM0->MCR |= SRImatchReg << 3*MatchReg;	
		}
		else if (MatchReg == 3){
			LPC_TIM0->MR3 = TimerInterval;
			LPC_TIM0->MCR |= SRImatchReg << 3*MatchReg;	
		}
		NVIC_EnableIRQ(TIMER0_IRQn);
		NVIC_SetPriority(TIMER0_IRQn, 4);	//Priority TIMER0
		return (0);
	}
	else if ( timer_id == 1 )
	{
		LPC_TIM1-> PR = Prescaler;
		
		if (MatchReg == 0){
			LPC_TIM1->MR0 = TimerInterval;
			LPC_TIM1->MCR |= SRImatchReg << 3*MatchReg;			
		}
		else if (MatchReg == 1){
			LPC_TIM1->MR1 = TimerInterval;
			LPC_TIM1->MCR |= SRImatchReg << 3*MatchReg;			
		}
		else if (MatchReg == 2){
			LPC_TIM1->MR2 = TimerInterval;
			LPC_TIM1->MCR |= SRImatchReg << 3*MatchReg;	
		}
		else if (MatchReg == 3){
			LPC_TIM1->MR3 = TimerInterval;
			LPC_TIM1->MCR |= SRImatchReg << 3*MatchReg;	
		}		
		NVIC_EnableIRQ(TIMER1_IRQn);
		NVIC_SetPriority(TIMER1_IRQn, 5);	//Priority TIMER1
		return (0);
	}
	else if ( timer_id == 2 )
	{
		LPC_TIM2-> PR = Prescaler;
		
		if (MatchReg == 0){
			LPC_TIM2->MR0 = TimerInterval;
			LPC_TIM2->MCR |= SRImatchReg << 3*MatchReg;			
		}
		else if (MatchReg == 1){
			LPC_TIM2->MR1 = TimerInterval;
			LPC_TIM2->MCR |= SRImatchReg << 3*MatchReg;			
		}
		else if (MatchReg == 2){
			LPC_TIM2->MR2 = TimerInterval;
			LPC_TIM2->MCR |= SRImatchReg << 3*MatchReg;	
		}
		else if (MatchReg == 3){
			LPC_TIM2->MR3 = TimerInterval;
			LPC_TIM2->MCR |= SRImatchReg << 3*MatchReg;	
		}		
		NVIC_EnableIRQ(TIMER2_IRQn);
		NVIC_SetPriority(TIMER2_IRQn, 6);	//Priority TIMER2
		return (0);
	}
	else if ( timer_id == 3 )
	{
		LPC_TIM3-> PR = Prescaler;
		
		if (MatchReg == 0){
			LPC_TIM3->MR0 = TimerInterval;
			LPC_TIM3->MCR |= SRImatchReg << 3*MatchReg;			
		}
		else if (MatchReg == 1){
			LPC_TIM3->MR1 = TimerInterval;
			LPC_TIM3->MCR |= SRImatchReg << 3*MatchReg;			
		}
		else if (MatchReg == 2){
			LPC_TIM3->MR2 = TimerInterval;
			LPC_TIM3->MCR |= SRImatchReg << 3*MatchReg;	
		}
		else if (MatchReg == 3){
			LPC_TIM3->MR3 = TimerInterval;
			LPC_TIM3->MCR |= SRImatchReg << 3*MatchReg;	
		}		
		NVIC_EnableIRQ(TIMER3_IRQn);
		NVIC_SetPriority(TIMER3_IRQn, 6);	//Priority TIMER3
		return (0);
	}
	
	return (1);
}



/******************************************************************************
**                            End Of File
******************************************************************************/