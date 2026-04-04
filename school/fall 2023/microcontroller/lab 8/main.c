/*******************************
Main
	initialize GPIO ports
	enable ADC clock
	enable USART clock
	Enable alternate modes
	set baud rate
	set parity bit
	set start bit
	set stop bit
	set oversampling
	set no hardware flow control
	
UART IRQHandler
	check RX
		put bit in buffer
	once done return buffer
	clear RX flag
	if buffer == 'T' or 't'
		call send data function
	clear buffer
	
Send data function
	get data from ADC
	put data in buffer
	send buffer bit by bit
	
	

*******************************/ 
#include "stm32l476xx.h"
#include <string.h>
#include <stdio.h>
#include "UART.h"
#define TS_CAL1 *((uint16_t *) 0x1FFF75A8)
#define TS_CAL2 *((uint16_t *) 0x1FFF75CA)



void ADC_Conversion_Seq(void){		
		// 1. Software can start one ADC conversion by setting the ADC_CR_ADSTART bit in the ADC1 ->CR register
		ADC1->CR |= ADC_CR_ADSTART;
		
		// 2. Software has to wait the completion of ADC conversion by checking whether ADC_CSR_EOC_MST in the
		// ADC123_COMMON->CSR register has been set by the hardware.
		while (!(ADC123_COMMON->CSR & ADC_CSR_EOC_MST));
		
		// 3. The conversion result is saved in register ADC1->DR.
}

void ADC1_Wakeup (void){ //for GPIO PA.1
    int wait_time;
    // To start ADC operations, the following sequence should be applied
    // DEEPPWD = 0: ADC not in deep-power down
    // DEEPPWD = 1: ADC in deep-power-down (default reset state)
    if ((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD) {
        // Exit deep power down mode if still in that state
        ADC1->CR &= ~ADC_CR_DEEPPWD;
    }

    // Enable the ADC internal voltage regulator
    // Before performing any operation such as Launching a calibration or
    // enabling the ADC, the ADC voltage regulator must first be enabled and
    // the software must wait for the regulator start-up time.
    ADC1->CR |= ADC_CR_ADVREGEN;

    // Wait for ADC voltage requlator start-up time. The software must wait for
    // the startup time of the ADC voltage regulator (T_ADCVREG_STUP, i.e., 20
    // us) before Launching a calibration or enabling the ADC.
    wait_time = 20 * (80000000 / 1000000);
    while (wait_time != 0) {
        wait_time--;
    }
		//wake the temperture sensor up
		ADC123_COMMON->CCR |= ADC_CCR_TSEN;
}

void printTemp(){
		double temperature, nt;
		char t_string[10];
		//read USART
		USART_Read(USART2, USART2_Buffer_Rx, 1);
		
		//checks input
		if(USART2_Buffer_Rx[0] == 'T' || USART2_Buffer_Rx[0] == 't' ) {
			strcpy(USART2_Buffer_Wx, "");
			strcpy(USART2_Buffer_Wx, "CPU Temperature is: ");
			//write above string to terminal
			USART_Write(USART2, USART2_Buffer_Wx, 20);
			//start temperature read
			ADC_Conversion_Seq();
			//scale for reference voltage
			temperature = ADC1->DR * (3.3/3.0);
			//calibration from manual
			nt = ((110.0 - 30.0)/(TS_CAL2 - TS_CAL1))*((temperature - TS_CAL1) + 148.0);
			
			//makes temperature into string
			sprintf(t_string, "%2.2f\r\n", nt);
			//writes string to terminal
			USART_Write(USART2, (uint8_t *)t_string, 7);
		}
		else{
			uint8_t string[32];
			//makes string
			strcpy(string, "Warning: Not a valid input.\n");
			//prints warning to terminal
			USART_Write(USART2, string, 28);
		} 
}


int main(void){
	RCC->CR |= RCC_CR_HSION; //turn on high speed clock
	while(RCC->CR != 0x00000563){} //wait till ready
	RCC->CFGR |= RCC_CFGR_SW_HSI; //switch to HSI
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //enable GPIOA
	RCC->CCIPR &=  ~(RCC_CCIPR_USART2SEL);
	RCC->CCIPR |=   (RCC_CCIPR_USART2SEL_1);
	GPIOA->MODER &= ~(0xF << (2*2)); // Clear mode bits for pin 2 and 3
	GPIOA->MODER |= 0xA << (2*2); //sets to alternate mode
	GPIOA->AFR[0] |= 0x77 << (4*2);//sets alternate mode
	GPIOA->OSPEEDR |= 0XF<<(2*2);//sets to high speed output
	GPIOA->PUPDR &= ~(0xF<<(2*2));//clears pupdr
	GPIOA->PUPDR |= 0x5<<(2*2); // Select pull-up

  // GPIO Output Type: 0 = push-pull, 1 = open drain
	GPIOA->OTYPER &= ~(0x3<<2);
	NVIC->ISER[1] = 1 << 6;//enable interupt
	//set up timer for USART
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	
	USART2->CR1 &= ~USART_CR1_UE;//disable
	USART2->CR1 &= ~USART_CR1_M;//set to 8 bits
	USART2->CR2 &= ~USART_CR2_STOP;//sets to 1 stop bit
	USART2->CR1 &= ~USART_CR1_PCE;//sets no parity
	USART2->CR1 &= ~USART_CR1_OVER8;//sets oversampling to 16
	USART2->CR1 |= (USART_CR1_TE | USART_CR1_RE);//enables Tx and Rx
	USART2->BRR = 0x683;//sets baud rate
	USART2->CR1 |= USART_CR1_UE;//enables USART
	//checks if USART is ready for transmision
	while ((USART2->ISR & USART_ISR_TEACK) == 0);
	//checks if USART is ready for reception
	while ((USART2->ISR & USART_ISR_REACK) == 0);
	
	
	//set bit 1 in ASCR to close the analog switch
	GPIOA->ASCR |= 0x2;
	
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
	//7.5 configure temp sensor
	ADC123_COMMON->CCR |= ADC_CCR_TSEN;
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
			ADC1->SQR1 |= ( 17U << 6 ); // PA1: ADC12_IN17
	//13. configure channel 6 as single ended: already single ended by default
	ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_17;
	//14. select ADC sample time
	ADC1->SMPR2 &= ~ADC_SMPR2_SMP17;
	ADC1->SMPR2 |= (ADC_SMPR2_SMP17_1 | ADC_SMPR2_SMP17_0);
	//15. select discontinuous mode
	ADC1->CFGR &= ~ADC_CFGR_CONT;
  ADC1->CFGR |= ADC_CFGR_DISCEN;
	//16. select software trigger
	ADC1->CFGR &= ~ADC_CFGR_EXTEN;
	//17. enable ADC1 
	ADC1->CR |= ADC_CR_ADEN;
	//18. wait until adc1 is ready
	while ((ADC1->ISR & ADC_ISR_ADRDY) == ADC_ISR_ADRDY);
		
		
	while(1){
		//function to print temp
		printTemp();
	}
}