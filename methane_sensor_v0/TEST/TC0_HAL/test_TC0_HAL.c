/*
 * test_TC0_HAL.c
 *
 * Created: 01-11-2021 11:03:12
 *  Author: Mainframe
 */ 

#include <xc.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#include "test_TC0_HAL.h"
#include "../../HAL/UART1/UART1_HAL.h"
#include "../../HAL/TC0/TC0_HAL.h"

//Functions
void test_blink_LED();

//Vars
uint16_t value=0;

void test_TC0_HAL_start(){
	
	uart1_hal_init();
	
	while(1){
		value=0;
		test_blink_LED();
		_delay_ms(2000);
	}
	
}

/************************************************************************/
/* Test callback and time                                               */
/************************************************************************/

void callback(){
	value++;
}

void test_blink_LED(){
	uart1_hal_send_string("Starting... ");
	TC0_HAL_init(150, &callback);
	_delay_ms(1000);
	uart1_hal_send_string("RUN ");
	TC0_HAL_start();
	_delay_ms(5000);
	TC0_HAL_stop();
	char msg[30];
	sprintf(msg, " %u ", value);
	uart1_hal_send_string(msg);
	_delay_ms(2000);
}