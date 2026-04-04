#ifndef __STM32L476R_NUCLEO_LCD_H
#define __STM32L476R_NUCLEO_LCD_H

void delay_ms(unsigned int ms);
void LCD_WriteCom(unsigned char com);
void LCD_WriteData(unsigned char dat);
void LCD_Init(void);
void LCD_Clear(void);
void LCD_DisplayString(unsigned int line, unsigned char *ptr);
void Putnibble(unsigned char data);
void Pulse(void);
void char_display(unsigned int c, unsigned int r);
void LCD_DisplayChar(unsigned char c);

#endif
