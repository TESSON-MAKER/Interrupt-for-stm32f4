#include "stm32f4xx.h"

static volatile int button_pressed = 0;

void EXTI15_10_IRQHandler(void) 
{
	if (EXTI->PR & EXTI_PR_PR13) // Check if it's the interrupt from PC13
	{  
		EXTI->PR |= EXTI_PR_PR13;   // Clear the interrupt flag

		// Manage debouncing
		if (!button_pressed) 
		{
			button_pressed = 1;

			// Add a short delay for debouncing (might need adjustment)
			for (volatile int i = 0; i < 100; ++i); 

			// Change the state of the LED connected to PA5
			GPIOA->ODR ^= GPIO_ODR_OD5;  // Toggle the state of pin PA5
		}
	}
	button_pressed = 0;
}

static void EXTI_Config(void) {
	// Enable clock for Ports C and A
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOAEN;

	// Configure PC13 (Push button) as input with pull-down
	GPIOC->MODER &= ~GPIO_MODER_MODE13; // Reset mode
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13; // Reset pull-up/pull-down
	GPIOC->PUPDR |= GPIO_PUPDR_PUPD13_0;  // Enable pull-down

	// Configure PA5 (LED) as output
	GPIOA->MODER &= ~GPIO_MODER_MODE5; // Reset mode
	GPIOA->MODER |= GPIO_MODER_MODE5_0;  // Configure as output

	// Configure external interrupt for PC13
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Enable SYSCFG clock
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC; // Select PC13 for EXTI13
	EXTI->IMR |= EXTI_IMR_IM13;     // Enable EXTI13 interrupt line
	EXTI->FTSR |= EXTI_FTSR_TR13;    // Configure to trigger interrupt on falling edge
	NVIC_EnableIRQ(EXTI15_10_IRQn); // Enable IRQ corresponding to EXTI15_10
	NVIC_SetPriority(EXTI15_10_IRQn, 0); // Set priority of IRQ EXTI15_10 to 0
}

int main(void) 
{
	EXTI_Config();
	GPIOA->ODR |= GPIO_ODR_OD5;

	while (1) 
	{
			//
	}
}
