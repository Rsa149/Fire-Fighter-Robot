#include "HC-SR04.h"

void ultra_init(void){
       /*Configuring PA5 for generating pulse sent to trig pin*/
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //Enable GPIOA clock
    GPIOA->CRL |= GPIO_CRL_MODE0; //Set the PA0 pin to output mode
    GPIOA->CRL &= ~GPIO_CRL_CNF0; //Set the PA0 pin to push-pull mode

    //Configuring output from echo pin to be sent to the board (PA6 pin)
    GPIOA->CRL &= ~GPIO_CRL_MODE1; //Set PA1 to input mode
    GPIOA->CRL &= ~GPIO_CRL_CNF1; //Set PA1 to floating input mode
	  GPIOA->CRL |= GPIO_CRL_CNF1_1;
}

void TIM2_us_Delay(uint32_t delay){
    RCC->APB1ENR |=1; //Start the clock for the timer peripheral
    TIM2->ARR = (int)(delay/0.0625); // Total period of the timer
    TIM2->CNT = 0;
    TIM2->CR1 |= 1; //Start the Timer
    while(!(TIM2->SR & TIM_SR_UIF)){} //Polling the update interrupt flag
    TIM2->SR &= ~(0x0001); //Reset the update interrupt flag
}

