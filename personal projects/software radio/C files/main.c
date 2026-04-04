/*************************
main
	initialize GPIO ports
	enable ADC clock
	enable alternate modes
	set oversampling
	set no hardware flow control
	
ADC data function
	get data from ADC
	put data in buffer
	sendbuffer bit by bit

*************************/

#include "stm32l476xx.h"
#include <string.h>
#include <stdio.h>
#include "UART.h"
#define TS_CAL1 *((uint16_t *) 0x1FFF75A8)
#define TS_CAL2 *((uint16_t *) 0x1FFF75CA)

int main(void){
	RCC->CR |= RCC_CR_HSION; //turn on high speed clock
	while(RCC->CR != 0x00000563){} //wait till ready
	RCC->CFGR |= RCC_CFGR_SW_HSI; //switch to HSI
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //enable GPIOA
	//figure out pins for ADC in and DAC output

  // GPIO Output Type: 0 = push-pull, 1 = open drain
	
	//initialize ADC 1
	//1. Enable ADC clock bit
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
	//2. disable ADC1
	ADC1->CR &= ~ADC_CR_ADEN;
	//3. enable i/o analog switches voltage booster
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN;
	//4. enable conversion of internal channels
	ADC123_COMMON->CCR |= ADC_CCR_VREFEN;
	//5. configure adc prescaler to zero
	ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;
	//6. configure synchronous clock mode
	ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE; //clean out what was in it
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;
	//7. configure all ADCs as independent
	ADC123_COMMON->CCR &= ~ADC_CCR_DUAL; 
	//8. wakeup ADC
	ADC1_Wakeup();
	//9. configure resolution to 12 bits: already default
	ADC1->CFGR &= ~ADC_CFGR_RES;
	//10. select alignment right: already default
	ADC1->CFGR &= ~ADC_CFGR_ALIGN;
	//11. select 1 conversion in the regular channel conversion sequence
	ADC1->SQR1 &= ~ADC_SQR1_L;
	//12. specify channel 6 as the first conversion in regular sequence
	// ADC regular sequence register 1 (ADC_SQRl)
			// eeeee: 1 conversion in the regular channeL conversion sequence
			ADC1->SQR1 &= ~ADC_SQR1_L;
		
			// Specify the channel number of the 1st conversion in regular sequence
			ADC1->SQR1 &= ~ADC_SQR1_SQ1;
			ADC1->SQR1 |= ( 5U << 6 ); // PA0: ADC12_IN5
	//13. configure channel 6 as single ended: already single ended by default
	ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_5;
	//14. select ADC sample time
	ADC1->SMPR1 &= ~ADC_SMPR1_SMP5;
	ADC1->SMPR1 |= (ADC_SMPR1_SMP5_1 | ADC_SMPR1_SMP5_0);
	//15. select discontinuous mode
	ADC1->CFGR &= ~ADC_CFGR_CONT;
	ADC1->CFGR |= ADC_CFGR_CONT;
	ADC1->CFGR &= ~ADC_CFGR_DISCEN;
	//16. select software trigger
	ADC1->CFGR &= ~ADC_CFGR_EXTEN;
	ADC1->CFGR |= ADC_CFGR_EXTEN_0 
	//17. enable ADC1 
	ADC1->CR |= ADC_CR_ADEN;
	//18. wait until adc1 is ready
	while ((ADC1->ISR & ADC_ISR_ADRDY) == ADC_ISR_ADRDY);
		
		
	while(1){
		//function to print temp
		printTemp();
	}
}