/*
 * TC1_HAL.c
 *
 * Created: 01-11-2021 15:48:35
 *  Author: Mainframe
 */ 

#include "TC1_HAL.h"
#include <xc.h>
#include <avr/interrupt.h>

#include "../../util/bit_operators.h"

static callback_ptr _cb;

void TC1_HAL_init(uint16_t match, callback_ptr cb){
	TCCR1A=0x00;
	TCCR1B=0b00001000; //Set CTC A mode
	TCCR1C=0x00;
	
	OCR1A=match; //Set compare value
	TCNT1=0x0000; //Set counter to zero
	
	set_bit(TIMSK1, 1); //Enable interrupt on compare
	sei();
	
	_cb=cb; //Set callback pointer
}

void TC1_HAL_start(){
	set_byte(TCCR1B, 0b00000110);
}

void TC1_HAL_stop(){
	clear_byte(TCCR1B, 0b00000111);
}

ISR(TIMER1_COMPA_vect){
	_cb();
}