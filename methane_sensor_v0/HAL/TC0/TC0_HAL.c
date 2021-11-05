/*
 * TC0_HAL.c
 *
 * Created: 01-11-2021 10:51:12
 *  Author: Mainframe
 */ 

#include <xc.h>
#include <avr/interrupt.h>

#include "TC0_HAL.h"
#include "../types.h"
#include "../../util/bit_operators.h"

callback_ptr _cb;






void TC0_HAL_init(uint8_t match, callback_ptr cb){
	TCCR0A = 0b00000010; //Set CTC mode
	TCCR0B= 0x00; //No clock (disabled)
	
	OCR0A=match; //Set match value
	TCNT0 = 0; //Set counter to 0
	
	set_bit(TIMSK0, 1); //enable compare interrupt
	sei(); //Enable global interrupt
	
	//Set callback
	_cb=cb;
	
}


void TC0_HAL_start(){
	TCCR0B=0b00000110; 
}

void TC0_HAL_stop(){
	TCCR0B=0x00; 
}

ISR(TIMER0_COMPA_vect){
	_cb();
}
