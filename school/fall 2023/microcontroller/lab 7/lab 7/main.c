#include "stm32l476xx.h"
#include "LCD.h"
#include <string.h>
#include <stdio.h>
#include "Keypad.h"
#include "StepperMotor.h"

/***********************************
initialize keypad
set up stepper matrix cases 
set up ISR

Turn on systick	
	systick->ctl = ####
	sets to system clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE
	set load value
	systick->LOAD = 15999999
		
keypad_timer
	read time from keypad
	wait for # press
	return time
	
systick handler
	get value from keypad_timer
	rotate stepper motor
	decrement time--
	when time = zero sound buzzer
	
motor_position
	get value from keypad_timer
	rotate motor to position
		
while (1)
{
	get time value from keypad_timer
	motor_position
	wait for second # press
	systick handler
}
***********************************/


int counting;//global variables to help program
int timer_val;
int last_pos;

void SysTick_Handler(void){
	
	if(timer_val != 0){
		last_pos = CountDown(last_pos);//move clock 1 second
		timer_val--;//decrement total time
	}else{
		counting = 0;//turn counting off
		}
	if(timer_val == 0){
		for(int i = 0; i != 1000; i ++){//loop for buzzer sound
			GPIOB->ODR |= 0x10;//turn on buzzer
			delay_ms(8);//leave on for 8 microseconds
			GPIOB->ODR &= 0xFFFFFFEF;//turn off buzzer
			delay_ms(4);//leave off for 4 microseconds
	}
	}
	
	SysTick->CTRL &= 0XFFFEFFFF;//disable systick flag
}
int main(void){
	Keypad_init();//initializes keypad and GPIO pins
	SysTick->LOAD = 15999999;//load value for 1 second systick
	SysTick->VAL = 0;//turns systick val to 0
	while(1){
		timer_val = Keypad_timer();//gets timer value from user
		last_pos = motor_position(timer_val);//sets clock to position
		counting =1;//turns on counting
		SysTick->CTRL |= 0X7;//turns on systick
		while(counting == 1){}//loop while decremting timer
			if(counting == 0){
			SysTick->CTRL &= 0XFFFFFFF8;//turns off systick
		}
	}
}