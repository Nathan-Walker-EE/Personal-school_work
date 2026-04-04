;******************** (C) Yifeng ZHU *******************************************
; @file    main.s
; @author  Nathan Walker and Easton Ewer
; @date    September-18-2023
;
;Pseudocode
;	load base GPIO register
;	offset to GPIO A and GPIO C
;	turn both ports clocks on
;	turn Port A to output mode
;	turn Port C to input mode
;	turn on pull down resistor 
;	
;	Debounce
;	if Port C is 1
;		then if port C is 1
;			Then if port C is 0
;				then if Port C is 0
;					if state counter < 2
;						then increment state counter
;						go to states
;					else 
;						state counter = 0
;						go to states
;				else go back
;			else stay
;		else go back
;	else wait
;					
;	States
;	Off = 0
;		set ODR to 0
;		go to Debounce
;	On = 1
;		Set ODR to have PA5 on
;		go to Debounce
;	Flash = 2
;		loop
;		turn ODR to 1
;		turn ODR to 0	
;		if IDR = 1
;		Debounce code
;			loop while button is pressed then turn off when released
;		
;	
;	
;*******************************************************************************


	INCLUDE core_cm4_constants.s
	INCLUDE stm32l476xx_constants.s
	
	AREA    main, CODE, READONLY
	EXPORT	__main
	ENTRY			
				