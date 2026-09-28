#include "stm32l476xx.h"

// PA.5  <--> Green LED
// PC.13 <--> Blue user button
#define LED_PIN    5
#define BUTTON_PIN 13

// User HSI (high-speed internal) as the processor clock
void enable_HSI(){
	// Enable High Speed Internal Clock (HSI = 16 MHz)
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);
  
	RCC->CFGR &= ~RCC_CFGR_SW; 
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
}

int main()
{
	//Start of Part A
	
	enable_HSI();
	
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //Enable clock of port A
	
	GPIOA -> MODER &= ~(3U<<(2*LED_PIN)); //Clears bit 10 and 11 for port A
	
	GPIOA -> MODER |= (1U<<(2*LED_PIN)); //Set bit 10, Set pin 5 as output
	
	GPIOA -> OTYPER &= ~(1U<<LED_PIN); //Set to push-pull on PA5
	
	GPIOA -> PUPDR &= ~(3U<<(2*LED_PIN)); //Set PA5 to no-pull-up and no-pull-down
	
	GPIOA -> ODR |= (1U<<LED_PIN); //Output 1 to turn on green LED
	
	//Start of Part B

	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOCEN; //Enable clock of port C
	
	GPIOC -> MODER &= ~(3U<<(2*BUTTON_PIN)); //Clears bit 26 and 27 for port C and set to input mode
	
	GPIOC -> PUPDR &= ~(3U<<(2*BUTTON_PIN)); //Set PC13 to no-pull-up and no-pull-down

	 while (1)
    {
        while (GPIOC->IDR & GPIO_IDR_IDR_13)
        {
        }
        GPIOA->ODR ^= (1U << LED_PIN); //Toggle the PA5 green LED once.

        while ((GPIOC->IDR & GPIO_IDR_IDR_13) == 0U) //Edge-triggered to stop the LED from toggling repeatedly
        {
        }
    }
}
