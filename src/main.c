#include "stm32f10x.h"

uint32_t data;
double time, dist;
void GPIO_Init(void);
void TIM2_us_Delay(uint32_t delay); /*TIM2 for generating 10us pulse for trig pin*/
void led_init(void);
void delay(uint32_t delay);
void red_on(void);
void green_on(void);
void red_off(void);
void green_off(void);

void GPIO_Init(void)
{
  /*Configuring PA5 for generating pulse sent to trig pin*/
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Enable GPIOA clock
  GPIOA->CRL |= GPIO_CRL_MODE0;       // Set the PA0 pin to output mode
  GPIOA->CRL &= ~GPIO_CRL_CNF0;       // Set the PA0 pin to push-pull mode

  // Configuring output from echo pin to be sent to the board (PA6 pin)
  GPIOA->CRL &= ~GPIO_CRL_MODE1; // Set PA1 to input mode
  GPIOA->CRL &= ~GPIO_CRL_CNF1;  // Set PA1 to floating input mode
  GPIOA->CRL |= GPIO_CRL_CNF1_1;
}

void TIM2_us_Delay(uint32_t delay)
{
  RCC->APB1ENR |= 1;            // Start the clock for the timer peripheral
  TIM2->ARR = (delay / 0.0625); // Total period of the timer
  TIM2->CNT = 0;
  TIM2->CR1 |= 1; // Start the Timer
  while (!(TIM2->SR & TIM_SR_UIF))
  {
  }                      // Polling the update interrupt flag
  TIM2->SR &= ~(0x0001); // Reset the update interrupt flag
}

void delay(uint32_t delay)
{
  while (delay--)
  {
  }
}
void led_init(void)
{

  GPIOA->CRL |= GPIO_CRL_MODE7_0 | GPIO_CRL_MODE7_1; // Third Led    PA7
  GPIOA->CRL &= ~GPIO_CRL_CNF7_0 & ~GPIO_CRL_CNF7_1;
  GPIOA->CRL |= GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1; // fOURTH Led    PA5
  GPIOA->CRL &= ~GPIO_CRL_CNF5_0 & ~GPIO_CRL_CNF5_1;
}
void red_on(void)
{
  GPIOA->ODR |= GPIO_ODR_ODR7;
  delay(1800);
}
void green_on(void)
{
  GPIOA->ODR |= GPIO_ODR_ODR5;
  delay(1800);
}
void red_off(void)
{
  GPIOA->ODR &= (uint32_t)~GPIO_ODR_ODR7;
  delay(1800);
}
void green_off(void)
{
  GPIOA->ODR &= (uint32_t)~GPIO_ODR_ODR5;
  delay(1800);
}

int main(void)
{
  RCC->CFGR |= 0 << 10; // set APB1 = 16 MHz
                        // RCC->CFGR = 0x00080000;// 16 MHz
  GPIO_Init();
  led_init();

  GPIOA->BSRR = 0x00000000; // Setting trig pin to low to initialize the module

  while (1)
  {
    // 1. Sending 10us pulse to
    GPIOA->BSRR &= 0xFFFFFFFE; // PA0 is low
    TIM2_us_Delay(2);
    GPIOA->BSRR |= 0x00000001; // PA0 set to High
    TIM2_us_Delay(10);         // Wait for 10us
    GPIOA->BSRR |= 0x00010000; // Make PA0 low again

    // 2. Measure the pulse width of the pulse sent from the echo pin by polling IDR for port A
    while (GPIOA->IDR & GPIO_IDR_IDR1)
    {
      data = data + 1;
    }

    // 3.Converting the gathered data into distance in cm
    if (data > 0)
    {

      time = data * (0.0625 * 0.000001);
      dist = ((time * 340) / 2) * 100;
    }

    if ((dist < 4))
    {
      green_off();
      red_on();
    }
    else
    {
      red_off();
      green_on();
    }

    // TIM2_us_Delay(4);
    data = 0;
  }
}
