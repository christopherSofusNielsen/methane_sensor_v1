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
#include "../../MODULES/RTC_module/RTC_module.h"
#include "../../HAL/TWI/TWI_HAL.h"
#include "../../util/bit_operators.h"

//Functions
static void test_blink_LED();
static void test_timing();

//Vars
static uint16_t value=0;

void test_TC1_HAL_start(){
	uart1_hal_init();
	
	while(1){
		value=0;
		//test_blink_LED();
		test_timing();
		_delay_ms(2000);
	}
}

static void callback(){
	value++;
}

/************************************************************************/
/* Test timing                                                          */
/************************************************************************/

static void test_timing(){
	
	//Set pull up
	set_bit(PORTB, 0);
	set_bit(PORTB, 1);
	
	TWI_HAL_init();
	RTC_STATUS status=RTC_set_clock_out(1);
	if(status != RTC_STATUS_SUCCESS){
		uart1_hal_send_string("Fail ");
		return;
	}
	
	TC1_HAL_init(10, &callback);
	uart1_hal_send_string("Start ");
	
	TC1_HAL_start();
	while(value==0){}
	uart1_hal_send_string("Stop ");
	TC1_HAL_stop();
}

/************************************************************************/
/* Test callback and time                                               */
/************************************************************************/

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

