#include "stm32l476xx.h"
#include "Keypad.h"

void Keypad_init(void){
	RCC->CR |= RCC_CR_HSION; 
	while(RCC->CR != 0x00000563){}
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	GPIOB->MODER &= 0xFFF00000;
	GPIOB->MODER |= 0x00000540;
	GPIOB->OTYPER |= 0x000003C0;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	GPIOC->MODER &= 0xFFFFFF00;
	GPIOC->PUPDR |= 0x00000055;
}

unsigned char Keypad_read(void){
	unsigned char p[4][4] = {{'1', '2', '3', 'A'},
							 {'4', '5', '6', 'B'}, 
							 {'7', '8', '9', 'C'}, 
							 {'*', '0', '#', 'D'}};
	while(1){
		input_read = GPIOC->IDR;
		input_read = input_read & 0x0000000F;
			for(i = 0; i < 4; i ++){
				if(((input_read >> i) & 0x1) == 0){
 					COLUMN = i + 1;
				}
			}
			if(COLUMN != 0){
				for(i = 0; i < 4; i ++){
					output_write = 0x000003B8  << i;
					output_write = output_write & 0x000003C0;
					GPIOB->ODR = GPIOB->ODR & 0xFFFFFC3F;
					GPIOB->ODR = GPIOB->ODR | output_write;
					delay_ms(5);
					input_read = GPIOC->IDR;
					if(input_read != 0xF){
						ROW = i + 1;
					}
				}
			}
			GPIOB->ODR = GPIOB->ODR & 0xFFFFFC3F;
			input_read = GPIOC->IDR;
			while(input_read != 0xF){
				input_read = GPIOC->IDR;
			}
			return p[ROW][COLUMN];
		}
	}
}

int Keypad_timer(void){
	int time;
	unsigned char time1;
	unsigned char time2;
	
	time1 = Keypad_read();
	time2 = Keypad_read();
	
	while(Keypad_read() != '#'){}
	
	time = (time2 - '0')*10 + (time1 - '0');
	return time;
}
