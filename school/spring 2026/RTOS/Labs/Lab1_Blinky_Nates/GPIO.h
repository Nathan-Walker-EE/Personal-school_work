#ifndef __STM32L476R_NUCLEO_GPIO_H

#include "stm32l476xx.h"

#define __STM32L476R_NUCLEO_GPIO_H

extern char time[3];

typedef enum{
	INPUT = 0U, OUTPUT = 1U, ALT_FUNC = 2U, ANALOG = 3U
}gpio_mode_t;

typedef enum{
	PUSHPULL = 0U, OPENDRAIN = 1U
}gpio_typer_t;

typedef enum{
	NO_PUPD = 0U, PULLUP = 1U, PULLDOWN = 2U
}gpio_pupdr_t;

typedef enum{
	LOW_SPEED = 0U, MEDIUM_SPEED = 1U, HIGH_SPEED = 2U, VERY_HIGH_SPEED = 3U
}gpio_speedr_t;

void clockEnable(GPIO_TypeDef *port);
void pinMode(GPIO_TypeDef *port, uint32_t pin, gpio_mode_t mode);
void setOutputType(GPIO_TypeDef *port, uint32_t pin, gpio_typer_t type);
void setPullUpDown(GPIO_TypeDef *port, uint32_t pin, gpio_pupdr_t pupd);
void setOutputSpeed(GPIO_TypeDef *port, uint32_t pin, gpio_speedr_t type);
void digitalWrite(GPIO_TypeDef *port, int pin, int value);
uint32_t digitalRead(GPIO_TypeDef *port, int pin);

void HSI_16Hz_Clocks_Init(void);

#endif
