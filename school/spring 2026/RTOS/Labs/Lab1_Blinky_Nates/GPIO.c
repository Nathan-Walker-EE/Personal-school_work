#include "GPIO.h"

#define INPUT 0
#define OUTPUT 1
#define ALT_FUNC 2
#define ANALOG 3

#define PUSHPULL 0
#define OPENDRAIN 1

#define NONE 0
#define PULLUP 1
#define PULLDOWN 2

#define LOW_SPEED 0      	//means bit 00
#define MEDIUM_SPEED 1     	//means bit 01
#define HIGH_SPEED 2		//means bit 10
#define VERY_HIGH_SPEED	3 	//means bit 10



//enable the clock depending on the port the user enters
void clockEnable(GPIO_TypeDef *port){
		if(port == GPIOA){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
		}
		else if(port == GPIOB){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
		}
		else if(port == GPIOC){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
		}
		else if(port == GPIOD){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
		}
		else if(port == GPIOE){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
		}
		else if(port == GPIOF){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN;
		}
		else if(port == GPIOG){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOGEN;
		}
		else if(port == GPIOH){
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOHEN;
		}
}

void pinMode(GPIO_TypeDef *port, uint32_t pin, gpio_mode_t mode) {
	//port->MODER = ...................
	// #define INPUT 0 - 00: Input mode
	// #define OUTPUT 1 - 01: General purpose output mode
	// #define ALT_FUNC 2 - 10: Alternate function mode
	// #define ANALOG 3 - 11: Analog mode (reset state)

	uint32_t temp = port->MODER;
	temp &= ~(0x3U << (pin*2));
	//temp |= (mode ? 1U : 0U) << (pin*2);
	temp |= mode << (pin*2);
	port->MODER = temp;
}

void setOutputType(GPIO_TypeDef *port, uint32_t pin, gpio_typer_t type) {
    //port->OTYPER = ...................
    // #define PUSHPULL OUTPUT 0
    // #define OPENDRAIN OUTPUT 1

	uint32_t temp = port->OTYPER;
	temp &= ~(0x1U << pin);
	//temp |= (type ? 1U : 0U) << pin;
	temp |= type << pin;
	port->OTYPER = temp;
}

void setPullUpDown(GPIO_TypeDef *port, uint32_t pin, gpio_pupdr_t pupd) {
    //port->PUPDR = ...................
    // #define NONE 0       means bit 00: No pull-up, pull-down
    // #define PULLUP 1     means bit 01: Pull-up
    // #define PULLDOWN 2   means bit 10: Pull-down

  uint32_t temp = port->PUPDR;
	temp &= ~(0x3U << (pin*2));
	temp |= pupd << (pin*2);
	port->PUPDR = temp;
}

void setOutputSpeed(GPIO_TypeDef *port, uint32_t pin, gpio_speedr_t pupd) {
    //port->PUPDR = ...................
    // #define LOW_SPEED 0      	means bit 00
    // #define MEDIUM_SPEED 1   	means bit 01
    // #define HIGH_SPEED 2   		means bit 10
	// #define VERY_HIGH_SPEED 3	means bit 11

  uint32_t temp = port->OSPEEDR;
	temp &= ~(0x3U << (pin*2));
	temp |= pupd << (pin*2);
	port->OSPEEDR = temp;
}

void digitalWrite(GPIO_TypeDef *port, int pin, int value) {
	uint32_t temp = port->ODR;
	uint32_t mask = 1 << pin;
	if(value){ //if were setting it high
		temp |= mask;
	}
	else{			//if were setting it low
		temp &= ~mask;
	}
	port->ODR = temp;
}

uint32_t digitalRead(GPIO_TypeDef *port, int pin) {
	uint32_t temp = port->IDR; //input cuz were reading
	temp = temp >> pin;
	temp &= 0x00000001;
	
	return temp;
}

void HSI_16Hz_Clocks_Init(void){
	//Enable the Internal high Speed oscillator (HSI)
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0); //wait for the clock to be ready
	uint32_t temp = 0x0;
	temp = temp & 0xFFFFFFFC;
	RCC->CFGR = temp | 0x00000001;
}


// example_uses
//		pinMode(GPIOA, 5, OUTPUT);
//		digitalWrite(GPIOA, 5, 1);
//		setOutputType(GPIOA, 5, OPENDRAIN);
//		pinMode(GPIOB, 3, INPUT);
//		setPullUpDown(GPIOB, 3, PULLUP)
//		int value = digitalRead(GPIOB, 3);
//		Clocks_Init();