#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "motor.h"
#include "fsm.h"

#include <string.h>
#include <stdio.h>

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

int main(void){
	char rxByte;
	int		n ;
	unsigned int initial_motor_delay; 
	
	System_Clock_Init(); // Switch System Clock = 80 MHz
	LED_Init();
	UART2_Init();   
  
  initial_motor_delay = motorInit();		
	
	// Initalize the two state machine objects
	init_SM( &motor0_SM, initial_motor_delay, 0, cmd_arr1);
	init_SM( &motor1_SM, initial_motor_delay, 1, cmd_arr2);
	
	configureSystemTick();
  
	Green_LED_On();
	
	while (1){
		rxByte = USART_Read(USART2);
		if (rxByte == 0xd) {
			n = sprintf((char *)buffer, "\r\n");
			USART_Write(USART2, buffer, n);	
		} else {
		n = sprintf((char *)buffer, "%s", &rxByte);
		USART_Write(USART2, buffer, n);		
		}
	}
}
