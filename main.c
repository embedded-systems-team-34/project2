#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "motor.h"
#include "fsm.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>

uint8_t buffer[BufferSize];
struct fsm motor0_SM;
struct fsm motor1_SM;


void TIM2_IRQHandler(void) {
    uint16_t which_interrupt = TIM2->SR;

    // Check for overflow interrupt
    if (((which_interrupt & TIM_SR_UIF) == TIM_SR_UIF)) {
     Red_LED_Toggle();
		 Green_LED_Toggle();
		 process_SM(&motor0_SM);
		 process_SM(&motor1_SM);
     TIM2->SR &= ~TIM_SR_UIF; // Clear overflow interrupt
    }
}

void configureSystemTick() {
	// Enable the interrupt handler
	NVIC_EnableIRQ(TIM2_IRQn); 
	
	// Enable clock of timer 2
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	
   // Set up TIM2 to generate system tick at 100 ms 
    
	// Set Prescaler
  // 80 MHz / 4000 = 20 KHz -> 50 us
  TIM2->PSC = 3999;
	
	TIM2->ARR = 2000;
  TIM2->EGR |= TIM_EGR_UG;
	
	// Unmask TIM2 Interrupts
	TIM2->DIER |= TIM_DIER_UIE;
	
	TIM2->CR1 |= TIM_CR1_CEN;
}

void parseUserInput(char *string,unsigned int length) {
    
    unsigned int double_help = 0;
    
    // Invalid command length so do nothing
    if (length != 2) {
        return;
    }
    // Length must be 2 so this is a valid command to parse
    
    // convert entered commands to uppercase 
    string[0] = toupper(string[0]); 
    string[1] = toupper(string[1]);
    
    // If double help is requested by user only print out a single help message instead of reapeating same message twice
    if ((string[0] == 'H') && (string[1] == 'H')) {
        double_help = 1;
    } else {
        double_help = 0;
    }
    
    // Only process the command if no 'x' are detected
    if ((string[0] != 'X') && (string[1] != 'X')) {
        parseSerialCommand(&motor0_SM, string[0]);
        // If Double help prevent displaying help message twice, else process state machine 1
        if (double_help == 0) {
            parseSerialCommand(&motor1_SM, string[1]);
        }
    }
}

void getLine() {
    
    char rxByte;
	int		n ;
    char userinput[100];
    unsigned int index = 0;
    
    n = sprintf((char *)buffer, "> ");
	USART_Write(USART2, buffer, n);
    index = 0;
    // Wait until the user enters a carrige return
    while(1) {
        // read the user input and echo it back to the screen
        rxByte = USART_Read(USART2);	
        if (rxByte == 0xd) {
					  userinput[index] = '\0';
            n = sprintf((char *)buffer, "\r\n");
            USART_Write(USART2, buffer, n);
            parseUserInput(userinput, index);
            break;
        }
        // echo user character back to the screen
        n = sprintf((char *)buffer, "%s", &rxByte);
        USART_Write(USART2, buffer, n);	
        userinput[index++] = (char)rxByte;
	}	
}

int main(void){
    
	unsigned int initial_motor_delay; 
	
	System_Clock_Init(); // Switch System Clock = 80 MHz
	LED_Init();
	UART2_Init();   
    
    printHelp();
    
    initial_motor_delay = motorInit();		
    
	// Initalize the two state machine objects
	init_SM( &motor0_SM, initial_motor_delay, 0, getRecipeStartAddress(0), getRecipeStartAddress(2));
	init_SM( &motor1_SM, initial_motor_delay, 1, getRecipeStartAddress(1), getRecipeStartAddress(3));
	
	configureSystemTick();
	
	while (1){
        getLine();
	}
}
