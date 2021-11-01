/*
 * test_TC1_HAL.c
 *
 * Created: 01-11-2021 16:19:49
 *  Author: Mainframe
 */ 


#include <xc.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#include "test_TC1_HAL.h"
#include "../../HAL/UART1/UART1_HAL.h"
#include "../../HAL/TC1/TC1_HAL.h"

//Functions
static void test_blink_LED();

//Vars
static uint16_t value=0;

void test_TC1_HAL_start(){
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
static void callback(){
	value++;
}

void test_blink_LED(){
	TC1_HAL_init(4000, &callback);
	uart1_hal_send_string("RUN ");
	TC1_HAL_start();
	_delay_ms(5000);
	TC1_HAL_stop();
	char msg[30];
	sprintf(msg, " %u ", value);
	uart1_hal_send_string(msg);
	_delay_ms(2000);
}

