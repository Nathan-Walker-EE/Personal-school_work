#include "stm32l476xx.h"
#include "StepperMotor.h"
#include "LCD.h"
#include "Keypad.h"


int motor_position(int timer_val)
{
	double degree; 
	int TotalSteps;
	int last_pos;
	
	degree = timer_val * 6;//gets time in terms of degrees
	TotalSteps = (int) ((degree * 64) / 5.625);//degrees to steps
	
	
	while (TotalSteps > 0)//while still needing to move
	{
		int STEP = TotalSteps % 8;//modulo 8 for case statement
		
		switch(STEP){//case statements for different half steps
	case 1: GPIOB->ODR &= 0xFFFFFFF0;
					GPIOB->ODR |= 0x8; 
					delay_ms(50);
					break;
	case 2: GPIOB->ODR &= 0xFFFFFFF0;
					GPIOB->ODR |= 0xC;
					delay_ms(50);
					break;			
	case 3: GPIOB->ODR &= 0xFFFFFFF0;
					GPIOB->ODR |= 0x4; 
					delay_ms(50);
					break;
	case 4: GPIOB->ODR &= 0xFFFFFFF0;
					GPIOB->ODR |= 0x6; 
					delay_ms(50);
					break;
	case 5: GPIOB->ODR &= 0xFFFFFFF0;
					GPIOB->ODR |= 0x2;
					delay_ms(50);
					break;
	case 6: GPIOB->ODR &= 0xFFFFFFF0;
					GPIOB->ODR |= 0x3;
					delay_ms(50);
					break;
	case 7: GPIOB->ODR &= 0xFFFFFFF0;
					GPIOB->ODR |= 0x1;
					delay_ms(50);
					break;
	case 0: GPIOB->ODR &= 0xFFFFFFF0;
					GPIOB->ODR |= 0x9;
					delay_ms(50);
					break;
		}
		last_pos = STEP;//gets last position
		TotalSteps--;//decrement total steps left
	}
	while(Keypad_read() != '#'){}//waits till pound press
	return last_pos;//return last pos
}

int CountDown (int last_pos)
{
	int STEP;
	int lp;
	
	for(STEP = last_pos+1; STEP < 69 + last_pos; STEP++){//decrements 68 times
		int temp;
		temp = STEP%8;//puts temp in case statement
		
		switch(temp){//case statements for halfsteping
	case 1: GPIOB->ODR &= 0xFFFFFFF0;
					GPIOB->ODR |= 0x8; 
					delay_ms(50);
					break;
	case 2: GPIOB->ODR &= 0xFFFFFFF0;
					GPIOB->ODR |= 0xC;
					delay_ms(50);
					break;			
	case 3: GPIOB->ODR &= 0xFFFFFFF0;
					GPIOB->ODR |= 0x4; 
					delay_ms(50);
					break;
	case 4: GPIOB->ODR &= 0xFFFFFFF0;
					GPIOB->ODR |= 0x6; 
					delay_ms(50);
					break;
	case 5: GPIOB->ODR &= 0xFFFFFFF0;
					GPIOB->ODR |= 0x2;
					delay_ms(50);
					break;
	case 6: GPIOB->ODR &= 0xFFFFFFF0;
					GPIOB->ODR |= 0x3;
					delay_ms(50);
					break;
	case 7: GPIOB->ODR &= 0xFFFFFFF0;
					GPIOB->ODR |= 0x1;
					delay_ms(50);
					break;
	case 0: GPIOB->ODR &= 0xFFFFFFF0;
					GPIOB->ODR |= 0x9;
					delay_ms(50);
					break;
		}
		lp = temp;//sets last position
	}
	return lp;//returns last position
}
	