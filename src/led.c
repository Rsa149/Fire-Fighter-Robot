#include "led.h"

void delay(uint32_t delay){while (delay--){}}
void led_init(void){
	  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //Enable GPIOA clock
	  GPIOA->CRL |=  GPIO_CRL_MODE7_0 |  GPIO_CRL_MODE7_1;     // Third Led    PA7
    GPIOA->CRL &= ~GPIO_CRL_CNF7_0 &~ GPIO_CRL_CNF7_1;
	  GPIOA->CRL |=  GPIO_CRL_MODE5_0 |  GPIO_CRL_MODE5_1;     // fOURTH Led    PA5
	  GPIOA->CRL &= ~GPIO_CRL_CNF5_0 &~ GPIO_CRL_CNF5_1;
}
void red_on(void){GPIOA->ODR |= GPIO_ODR_ODR7;delay(1800);}
void green_on(void){GPIOA->ODR |= GPIO_ODR_ODR5;delay(1800);}
void red_off(void){GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR7;delay(1800);}
void green_off(void){GPIOA->ODR &= (uint32_t) ~GPIO_ODR_ODR5;delay(1800);}