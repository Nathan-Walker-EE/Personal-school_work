#include "stm32l476xx.h"
#include "Keypad.h"
#include "LCD.h"

volatile unsigned int input_read;
volatile unsigned int output_write;
int COLUMN;
int ROW;
int i;

void Keypad_init(void){
	RCC->CR |= RCC_CR_HSION; //turn on high speed clock
	while(RCC->CR != 0x00000563){} //wait till ready
	RCC->CFGR |= RCC_CFGR_SW_HSI; //switch to HSI
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; //enable GPIOB
	GPIOB->MODER &= 0xFFF00000;//turn to input mode
	GPIOB->MODER |= 0x00055155;//turn to output mode
	GPIOB->OTYPER |= 0x000003C0;//turn to open drain
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;//enable GPIOC
	GPIOC->MODER &= 0xFFFFFF00;//turn to input mode
}
unsigned char Keypad_read(void){
	//char for keypad
	unsigned char p[4][4] = {{'1', '2', '3', 'A'},
							 {'4', '5', '6', 'B'}, 
							 {'7', '8', '9', 'C'}, 
							 {'*', '0', '#', 'D'}};
	int COLUMN = Column_read();//reads column
	int ROW = Row_read();	//reads row
	return p[ROW-1][COLUMN-1];//returns char from array
}

int Column_read(void){
	int COLUMN = 0;
	while(COLUMN == 0){//column is still 0
		input_read = GPIOC->IDR;//reads IDR
		input_read = input_read & 0x0000000F;//masks other bits but first 4
		for(i = 0; i < 4; i ++){//loop for four columns
			if(((input_read >> i) & 0x1) == 0){//checks one row at a time
 				COLUMN = i + 1;//COLUMN = i + 1 for physical row
				break;//leave for loop
			}
		}
	}
	return COLUMN;//returns COLUMN
}

int Row_read(void){
	int ROW = 0;
	for(i = 0; i < 4; i ++){//checks row
		output_write = 0x000003B8  << i;//cycles 0
		output_write = output_write & 0x000003C0;//masks output write
		GPIOB->ODR = GPIOB->ODR & 0xFFFFFC3F;//zeros ODR
		GPIOB->ODR = GPIOB->ODR | output_write;//puts in 0 from output_write
		delay_ms(3);//waits for IDR to sense
		input_read = GPIOC->IDR;//reads IDR
		if(input_read != 0xF){//checks row for press
			ROW = i + 1;//sets physical row
			break;
		}
	}
	GPIOB->ODR = GPIOB->ODR & 0xFFFFFC3F;//sets all to 0
	input_read = GPIOC->IDR;//reads IDR
	while(input_read != 0xF){//checks for button press still
		input_read = GPIOC->IDR;//reads IDR
	}
	return ROW;//returns ROW
}

int Keypad_timer(void){
	int time;
	unsigned char time1;
	unsigned char time2;
	int T1;
	int T2;
	
	time1 = Keypad_read();//gets 10's number
	time2 = Keypad_read();//gets 1's number
	
	while(Keypad_read() != '#'){}//waits till pound press
	T1 = time1 - 48;//gets int value
	T2 = time2 - 48;//gets int value
	time = T1 * 10 + T2;//turns int values to double digit
	return time;
}
