#include "LCD.h"
#include "stm32l476xx.h"

void delay_ms(unsigned int ms) {
	unsigned int i, j;
	for(i = 0; i < ms; i++){
		for(j = 0; j < 50; j++){
			/*j = j;*/
		}
	}
}
/***************
	set RS to 0
	Send upper 4 bits of command
	pulse
	send lower 4 bits of command
	wait
***************/
void LCD_WriteCom(unsigned char com) {
	GPIOB->ODR &= 0xffffffEf;
	Putnibble(com>>4);
	Pulse();
	Putnibble(com & 0x0f);
	Pulse();
	GPIOB->ODR &= 0xFFFFFFC0;
}
/**************
	set RS to 1
	Send upper 4 bits of data
	wait
	send lower 4 bits of data
	wait
**************/
void LCD_WriteData(unsigned char dat) {
	GPIOB->ODR |= 0x00000010;
	Putnibble(dat>>4);
	Pulse();
	Putnibble(dat &0x0f);
	Pulse();
}

/***************
Initialize LCD
	enable clock
	wait
	enable GPIO clocks
	configure GPIO pins
	set up 4bit bus, 2 line display, 5x8 dot matrix
	display on, cursor off, blinking off
	cursor move left-right, no display scrolling
	clear display
	set up CGRAM adddress to start at zero
***************/
void LCD_Init(void){
	RCC->CR |= RCC_CR_HSION; /*turn on clock, wait for ready, turn on switch */
	while(RCC->CR != 0x00000563){}
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	GPIOB->MODER &= 0xFFF00000;
	GPIOB->MODER |= 0x00005555;
	GPIOB->OTYPER |= 0x000003C0;
	GPIOB->ODR = GPIOB->ODR & 0xFFFFFC3F;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	GPIOC->MODER &= 0xFFFFFFC0;
	GPIOC->PUPDR |= 0x0000002A;
	LCD_WriteCom(0x28);
	LCD_WriteCom(0x0C);
	LCD_WriteCom(0x06);
	LCD_WriteCom(0x14);
	LCD_WriteCom(0x40);
}

void LCD_Clear(void){
  LCD_WriteCom(0x01);
}

void LCD_DisplayString(unsigned int line, unsigned char *ptr) {
	int i;
	unsigned char cmd;
	if(line == 1){
		cmd = 0x80 + 0x40;
	} else	
			cmd = 0x80;
	LCD_WriteCom(cmd);
	for(i = 0;ptr[i] != '\0'; i++){
		LCD_WriteData(ptr[i]);
	}
}

void LCD_DisplayChar(unsigned char c) {
	static int NumPos = 0; 
	static int done = 0;
	unsigned char cmd;
	
	if(NumPos <= 16 && done == 0) {
		cmd = 0x80;
		done = 1;
		LCD_WriteCom(cmd);
	}
	else if(NumPos > 16 && done == 1){
		cmd = 0x80+0x40;
		LCD_WriteCom(cmd);
		done++;
	}

	LCD_WriteData(c);
	NumPos++;

}

void Putnibble(unsigned char data){
	GPIOB->ODR &= 0xf0;
	GPIOB->ODR |= data;
}

void Pulse(void){
	GPIOB->ODR |= 0x20;
	delay_ms(25);
	GPIOB->ODR &=0xffffffDF;
	delay_ms(25);
}
