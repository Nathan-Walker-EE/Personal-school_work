#include "stm32L476xx.h"

#include "FreeRTOS.h"
#include "task.h"

#include "GPIO.h"

#define HIGH 1
#define LOW 0

int led_on_flag = 0;

void prvSetupHardware(void){
	clockEnable(GPIOA);
	clockEnable(GPIOC);
	
	pinMode(GPIOA, 5, OUTPUT); // LED
	pinMode(GPIOC, 13, INPUT); // Button
	
	digitalWrite(GPIOA, 5, LOW); //start with LED OFF

}

void vReadButton() {
	while (1) {
		if (digitalRead(GPIOC, 13) == 1)
			led_on_flag = 1;
		else
			led_on_flag = 0;
	}
}

void vSetLED() {
	while (1) {
		if (led_on_flag == 1)
			digitalWrite(GPIOA, 5, LOW);
		else
			digitalWrite(GPIOA, 5, HIGH);
	}
}

int main( void )
{
	/* Perform any hardware setup necessary. */
	prvSetupHardware();

	/* --- APPLICATION TASKS CAN BE CREATED HERE --- */

	xTaskCreate( vReadButton,
                 "READ_BUTTON",
                 32,
                 NULL,
                 tskIDLE_PRIORITY,
                 NULL );
	
	xTaskCreate( vSetLED,
                 "CHANGE_LED",
                 32,
                 NULL,
                 tskIDLE_PRIORITY,
                 NULL );

	/* Start the created tasks running. */
	vTaskStartScheduler();
	
	/* Execution will only reach here if there was insufficient heap
	to start the scheduler. */

	// digitalWrite(GPIOA, 5, HIGH);
	//digitalWrite(GPIOA, 5, LOW);
	for( ;; );

	return 0;
}