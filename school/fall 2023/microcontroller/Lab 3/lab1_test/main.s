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
;		Turn on pull down resistors for input pins
;				
;
;
;		COUNTING
;		loop
;			N + 1
;			if IDR = 2
;			go to PAUSED
;			if IDR = 1
;			branch link RESET
;			if N = ~156000
;				ODR - 1
;				N = 0
;			if ODR == 0
;			branch link RESET
;			else
;			go to COUNTING	
;		end 
;
;		PAUSED
;			if IDR = 4 
;			go to COUNTING
;			else
;			go to PAUSED
;
;		RESET
;			reset ODR to 0b00000000000000000000001111111111
;
;		
;			
;		DEBOUNCE
;		DEBOUNCER CODE
;		DEBOUNCE
;		WAITING LDR r5, [r2, #GPIO_IDR] ; gets value from port C IDR
;		CMP r5, #0x0000 ; checks if button is pressed
;  		BEQ PUSHED ; if button is pressed go to next state of DEBOUNCER0
;	then_WAITING B WAITING ; if button is not pressed go back to WAITING
;
;		PUSHED LDR r5, [r2, #GPIO_IDR] ; gets value from port C IDR
;		CMP r5, #0x0000 ; checks if button is pressed
;		BEQ HELD ; goes to held if button is done bouncing
;	then_PUSHED B 	WAITING ; goes back to waiting if button is still bouncing
;
;		HELD LDR r5, [r2, #GPIO_IDR] ; gets value from port C IDR
;		CMP r5, #0x0000 ; checks if button is pressed
;		BEQ HELD ; stays at held if button is still pressed
;	then_HELD B RELEASED ; goes to RELEASED once button is let go
;
;		RELEASED LDR r5, [r2, #GPIO_IDR] ; gets value from port C IDR
;		CMP r5, #0x0000 ; checks if button is pressed
;		BEQ HELD ; goes back to held for button bouncing
;	then_RESET CMP r4, #0x0002 ; if button is done bouncing check state counter
;		BEQ COUNTER_RESET ; if state counter is equal to 2 go to COUNTER_RESET
;	then_INCREMENT ADD r4, r4, #1 ; state counter is not equal to zero increment state
;		B OFFSTATE ; goes back to LED state section of code
;	
;	
;		COUNTER_RESET LDR r4, =0x00000000 ; loads 0 into state counter
;		B OFFSTATE; goes back to LED state section of code
;
;
;
;
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
   ORR r6, r6, #RCC_AHB2ENR_GPIOBEN; Enables port B with port C
   ORR r1, r1, r6; puts the value of r2 into r1 so that both port A and C clocks will be enabled
   STR r1, [r0, #RCC_AHB2ENR]; puts the value back into memory so the CPU knows which ports to enable
   
   ; Turn Port C to output mode
   LDR r0, =GPIOC_BASE ; set r0 to the base address for the GPIO port A
	LDR r1, [r0, #GPIO_MODER] ; set r1 to the value of r0 offset by the MODER constant
	ldr r4, =0xFFF00000
	AND r1, r1, r4
	LDR r4, =0x00055555
	ORR r1, r1, r4 
	STR r1, [r0, #GPIO_MODER] ; store the value of r1 back into memory at the address for MODER
	LDR r1, [r0, #GPIO_OTYPER]
	ldr r4, =0x000003FF
	ORR	r1, r1, r4
	STR r1, [r0, #GPIO_OTYPER]
	LDR r1, [r0, #GPIO_ODR] ; set r1 to r0 offset by the constant to get the Output Data Register value
	
	; Turn Port B to input mode
	LDR r2, =GPIOB_BASE ; set r0 to the base address for the GPIO port C
	LDR r3, [r2, #GPIO_MODER] ; set r1 to the value of r0 offset by the MODER constant
	orr r3, r3, #0xFFFFFFFF
	AND r3, r3, #0xFFFFFFC0 
	STR r3, [r2, #GPIO_MODER] ; store the value of r1 back into memory at the address for MODER
	LDR r3, [r2, #GPIO_PUPDR]
	ORR r3, r3, #0x0000002A
	STR r3, [r2, #GPIO_PUPDR]
	LDR r3, [r2, #GPIO_IDR] 
	

	MOV r3, #0x0
COUNTING	
	ADD r3, #1
	LDR r5, [r2, #GPIO_IDR]
	CMP r5 , #0x2
	BEQ PAUSED
	CMP	r5 , #0x1
	BEQ RESET_C
RESUME_C
	ldr r4, =0x1e848
	CMP r3, r4
	BEQ N_RST
	CMP r1 , #0x0
	BEQ RESET_C
	B COUNTING
	
PAUSED
	LDR r5, [r2, #GPIO_IDR]
	CMP r5, #0x4
	BEQ N_RST
	CMP r5, #0x1
	BEQ	RESET_P
	B PAUSED
	
N_RST
	MOV r3, #0x0
	SUB r1, #0x1
	STR r1, [r0, #GPIO_ODR]
	B COUNTING
		
	
	
	
RESET_C
	MOV r1, #0x3FF
	STR r1, [r0, #GPIO_ODR]
	B RESUME_C
	
RESET_P
	MOV r1, #0x3ff
	STR r1, [r0, #GPIO_ODR]
	B PAUSED
	   

	ENDP ; return 0
					
	ALIGN			

	END ; end