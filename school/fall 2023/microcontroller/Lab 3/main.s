;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Lab 3 main.s file
;
;	Authors: Easton Ewer and Nathan Walker
;	Goal: Make a 10 bit counter with a 
;	on button, pause button, and reset button
;
;	Pseudocode
;		
;		;Turn on output/input pins
;			enable clock for 13 pins
;			turn 10 pins to output mode
;			turn 3 pins to input mode
;			enable open drain for output pins
;
;		Turn on pull up resistors for all pins
;		
;		Turn MSI clock off
;		Turn clock to 100khz
;		Turn MSI clock on
;
;		loop
;			N + 1
;		end loop when arithmetic until N = ~156000
;		go to LED control
;
;
;		LED Control
;			ODR - 1
;			if ODR == 0
;			reset ODR to 0b00000000000000000000001111111111
;			else
;			go to WAIT
;
;
;
;
;		Debounce code from Lab 2 
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
INCLUDE core_cm4_constants.s
	INCLUDE stm32l476xx_constants.s
	
	AREA    main, CODE, READONLY
	EXPORT	__main
	ENTRY			
				
__main	PROC ; begin
		
   ; Enable the clock to GPIO port C
   LDR r0, =RCC_BASE ; set r0 to the base address for the clock 
   LDR r1, [r0, #RCC_AHB2ENR] ; loads the current value of enable for the clocks
   LDR r6, =RCC_AHB2ENR_GPIOCEN ; loads value needed for port A to be enabled
   ORR r1, r1, r6; puts the value of r2 into r1 so that both port A and C clocks will be enabled
   STR r1, [r0, #RCC_AHB2ENR]; puts the value back into memory so the CPU knows which ports to enable
   
   ; Turn Port C to output mode
   LDR r0, =GPIOC_BASE ; set r0 to the base address for the GPIO port A
	LDR r1, [r0, #GPIO_MODER] ; set r1 to the value of r0 offset by the MODER constant
	AND r1, r1, #0xFFFFF3FF ; AND the value of r1 with the constant 0xFFFFF3FF to make the pin controlling the LED go to input mode (00)
	ORR r1, r1, #0x00000400 ; OR the value of r1 with the constant 0x00000400 to make the pin controlling the LED go to output mode (01)
	STR r1, [r0, #GPIO_MODER] ; store the value of r1 back into memory at the address for MODER
	LDR r1, [r0, #GPIO_ODR] ; set r1 to r0 offset by the constant to get the Output Data Register value
	
	; Turn Port C to input mode
	LDR r2, =GPIOC_BASE ; set r0 to the base address for the GPIO port C
	LDR r3, [r2, #GPIO_MODER] ; set r1 to the value of r0 offset by the MODER constant
	AND r3, r3, #0xF3FFFFFF ; AND the value of r1 with the constant 0xFFFFF3FF to make the pin controlling the BUTTON go to input mode (00)
	STR r3, [r2, #GPIO_MODER] ; store the value of r1 back into memory at the address for MODER
	LDR r3, [r2, #GPIO_IDR] 
   
   

	ENDP ; return 0
					
	ALIGN			

	END ; end