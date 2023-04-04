# include "stm32f10x.h"
# include "HC-SR04.h"
# include "led.h"
# include "servo.h"
# include "wheel.h"

uint32_t dt ;
uint32_t data;
double time,dist,d_left,d_right;
double distance();
double distance1();


int main(){
	 ultra_init();
	 led_init();
	 wheel_init();
	
	/****for servo ******/
	clockInit();         
	tim3_IO_init();      
	tim3GPIOSetup ();
	/********************/
	
	
	float prev_dist = 0;
	int loop = 1;
	GPIOA->BSRR = 0x00000000;// Setting trig pin to low to initialize the module
   while(loop){
       dist = distance();
			if (dist > 0 && fabs(dist - prev_dist) > 0.1){
				//----------------------------------------------------------------------------------------
						if (dist < 1){		
								stop();
							moveservo(180);
									float prev_dist_left = 0;
									int left_loop = 1;
								GPIOA->BSRR = 0x00000000;// Setting trig pin to low to initialize the module
									while(left_loop){
									d_left = 0;
									 d_left = distance();
										if (d_left > 0 && fabs(d_left - prev_dist_left) > 0.1){
											if (d_left < 0.8){
															moveservo(10);	
															turn_right();
												break;
													}
													else{ 
														moveservo(90);
														turn_left();
														break;
													}
													 prev_dist_left = d_left;
												}
										dt = 0;
									}												
							}
						//---------------------------------------------------------------------------------
						else{
							red_off();
							green_on();	
							moveservo(90);
							left_wheel_forward_on();
						  right_wheel_forward_on();
					}
				  prev_dist = dist;
				}
        data = 0;				
   }  // while loop ends
}     // main ends



























uint32_t da;
double ti,di;
double distance(){
	
	 //1. Sending 10us pulse to
      GPIOA->BSRR &= 0xFFFFFFFE; // PA0 is low
			TIM2_us_Delay(2);
			GPIOA->BSRR |= 0x00000001; // PA0 set to High
			TIM2_us_Delay(10);         // Wait for 10us
			GPIOA->BSRR |= 0x00010000; // Make PA0 low again

        //2. Measure the pulse width of the pulse sent from the echo pin by polling IDR for port A
        while (GPIOA->IDR & GPIO_IDR_IDR1){
				da = da + 1;
					}
			
        //3.Converting the gathered data into distance in cm
        if (da>0){
				
            ti = da*(0.0625*0.000001);
            di = ((ti*340)/2)*100;
        }
				else{
        di = 0;
				}
			
       // TIM2_us_Delay(4);
        da = 0;
				return di;
		
}

uint32_t da1;
double ti1,di1;
double distance1(){
	
	 //1. Sending 10us pulse to
      GPIOA->BSRR &= 0xFFFFFFFE; // PA0 is low
			TIM2_us_Delay(2);
			GPIOA->BSRR |= 0x00000001; // PA0 set to High
			TIM2_us_Delay(10);         // Wait for 10us
			GPIOA->BSRR |= 0x00010000; // Make PA0 low again

        //2. Measure the pulse width of the pulse sent from the echo pin by polling IDR for port A
        while (GPIOA->IDR & GPIO_IDR_IDR1){
				da1 = da1 + 1;
					}
			
        //3.Converting the gathered data into distance in cm
        if (da1>0){
				
            ti1 = da*(0.0625*0.000001);
            di1 = ((ti1*340)/2)*100;
        }
				else{
        di1 = 0;
				}
			
       // TIM2_us_Delay(4);
        da1 = 0;
				return di1;
		
}