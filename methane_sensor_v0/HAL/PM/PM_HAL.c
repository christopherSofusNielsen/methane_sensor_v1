/*
 * PM_HAL.c
 *
 * Created: 18-10-2021 09:59:05
 *  Author: Mainframe
 */ 

#include <xc.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "PM_HAL.h"
#include "../../util/bit_operators.h"

void PM_HAL_adc_power_init(){
	set_bit(DDRA, 3);
	clear_bit(PORTA, 3);
}

void PM_HAL_adc_power(bool on){
	if(on){
		set_bit(PORTA, 3);
	}else{
		clear_bit(PORTA, 3);
	}
}

void PM_HAL_meth_power_init(){
	set_bit(DDRA, 5);
	clear_bit(PORTA, 5);
}

void PM_HAL_meth_power(bool on){
	if(on){
		set_bit(PORTA, 5);
	}else{
		clear_bit(PORTA, 5);
	}
}

void PM_HAL_SCD30_power_init(){
	set_bit(DDRA, 4);
	clear_bit(PORTA, 4);
}

void PM_HAL_SCD30_power(bool on){
	if(on){
		set_bit(PORTA, 4);
	}else{
		clear_bit(PORTA, 4);
	}
}

void PM_HAL_BC_power_init(){
	set_bit(DDRD, 4);
	clear_bit(PORTD, 4);
}

void PM_HAL_BC_power(bool on){
	if(on){
		set_bit(PORTD, 4);
	}else{
		clear_bit(PORTD, 4);
	}
}

void PM_HAL_LED_power_init(){
	set_bit(DDRA, 7);
	clear_bit(PORTA, 7);
}

void PM_HAL_LED_power(bool on){
	if(on){
		set_bit(PORTA, 7);
	}else{
		clear_bit(PORTA, 7);
	}
}

void PM_HAL_LED_toggle(){
	toggle_bit(PORTA, 7);
}

void PM_HAL_enter_power_down(){
	clear_bit(DDRB, 2); //set PB2 as input
	set_bit(PORTB, 2); //Enbale pull up
	
	//Set faling edge interrupt
	clear_bit(EICRA, 4);
	set_bit(EICRA, 5);
	
	//Enable interrupt
	set_bit(EIMSK, 2);
	
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sei();
	sleep_cpu();
	sleep_disable();
	clear_bit(EIMSK, 2);
}

ISR(INT2_vect){
	
}
