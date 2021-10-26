/*
 * PM_HAL.c
 *
 * Created: 18-10-2021 09:59:05
 *  Author: Mainframe
 */ 

#include <xc.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "PM_HAL.h"
#include "../../util/bit_operators.h"

void PM_HAL_enter_power_down(){
	clear_bit(DDRD, 2); //set PD2 as input
	set_bit(PORTD, 2); //Enbale pull up
	
	//Set faling edge interrupt
	clear_bit(EICRA, 0);
	set_bit(EICRA, 1);
	
	//Enable interrupt
	set_bit(EIMSK, 0);
	
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sei();
	sleep_cpu();
	sleep_disable();
	clear_bit(EIMSK, 0);
}

ISR(INT0_vect){
	
}
