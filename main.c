#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"

#include <string.h>
#include <stdio.h>

uint8_t buffer[BufferSize];

void TIM2_IRQHandler(void) {
    uint16_t which_interrupt = TIM2->SR;

    // Check for overflow interrupt
    if (((which_interrupt & TIM_SR_UIF) == TIM_SR_UIF)) {
     Red_LED_Toggle();
		 Green_LED_Toggle();
     TIM2->SR &= ~TIM_SR_UIF; // Clear overflow interrupt
    }
}

int main(void){
	char rxByte;
	int		n ;
	
	System_Clock_Init(); // Switch System Clock = 80 MHz
	LED_Init();
	UART2_Init();
	
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
    
    // set up Tim5 for PWM on channel 1 (PA0) and 2 (PA1)
    
    // Configure PA0 for alternate function
    // Enable clk to PortA
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    
    // Set PA0 to be alternate function
    GPIOA->MODER &= ~GPIO_MODER_MODER0;        // Clear moder register mode0[1:0]
    GPIOA->MODER |= GPIO_MODER_MODER0_1;    // Set alternate function mode 10
    
    // Set Alternate function lower register to AF2 so that A0 is set connected to TIM5_CH1
    GPIOA->AFR[0] = 0x2;
		
    // Set PA1 to be alternate function
    GPIOA->MODER &= ~GPIO_MODER_MODER1;        // Clear moder register mode0[1:0]
    GPIOA->MODER |= GPIO_MODER_MODER1_1;    // Set alternate function mode 10		
		
		// Set Alternate function lower register to AF2 so that A0 is set connected to TIM5_CH1
    GPIOA->AFR[0] |= 0x20;
    
    // Enable clock for timer 5
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;
    
    // Set Prescaler
    // 80 MHz / 4000 = 20 kHz -> 50 us
    TIM5->PSC = 3999;
    
    // Set duty cycle period 
    // 50 us * 400 = 20 ms
    TIM5->ARR = 399;
    TIM5->EGR |= TIM_EGR_UG;
    // Write TIM5_CCR1 to control the duty cycle
    // Set a duty cycle of 2 ms
    TIM5->CCR1 = 39;
		TIM5->CCR2 = 79;
    
    // Set PWM mode 1, channel 1 is active as long as TIMx->CNT < TIMx->CCR1
    TIM5->CCMR1 |= 0x60;
		// Set PWM mode 1, channel 1 is active as long as TIMx->CNT < TIMx->CCR1
		TIM5->CCMR1 |= 0x3000;
		// Enable Output compare on channel 1
    TIM5->CCER |= 1;
		TIM5->CCER |= 0x10;
    TIM5->CR1 |= TIM_CR1_CEN;
    
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