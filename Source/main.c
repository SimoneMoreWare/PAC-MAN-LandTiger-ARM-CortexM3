#include "LPC17xx.h" // Include header for LPC17xx microcontroller definitions

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // External flag for simulator scaling (if applicable)
#endif

#include "GLCD/GLCD.h"       // Include header for graphical LCD functions
#include "timer/timer.h"     // Include header for timer functions
#include "main/game/game_initializer.h" // Include header for Pac-Man game initialization

int main(void)
{
    // Initialize system clock and other low-level settings
    SystemInit();
	
		CAN_Init();
    
    // Initialize and configure the LCD display
    LCD_Initialization();
    LCD_Clear(Black); // Clear the screen and set background color to black
    
    // Initialize joystick inputs for user controls
    joystick_init();
    
    // Initialize buttons for additional user input
    button_init();
	
		// Enable Buzzer
		speaker_init();
			
    // Enable power for TIMER2 and TIMER3 peripherals
    LPC_SC -> PCONP |= (1 << 22);  // Power up TIMER2
    LPC_SC -> PCONP |= (1 << 23);  // Power up TIMER3
    
    // Configure the Repetitive Interrupt Timer (RIT) for a 50ms interval
    init_RIT(0x004C4B40); // Load timer with 50ms interval
    enable_RIT();         // Enable the RIT
    
    // Initialize custom game timers
    initialize_game_timers();
    
    // Initialize the Pac-Man game
    initialize_pacman_game();
		

    // Enter power-down mode (low-power mode)
    LPC_SC->PCON |= 0x1;  // Set the PCON register to enable power-down
    LPC_SC->PCON &= ~(0x2); // Clear the deep-sleep bit
    
    // Main loop
    while (1)    
    {
        __ASM("wfi"); // Wait for interrupt instruction to reduce power consumption
    }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
