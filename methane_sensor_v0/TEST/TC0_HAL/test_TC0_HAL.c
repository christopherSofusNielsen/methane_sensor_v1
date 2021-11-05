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
#include "../../MODULES/RTC_module/RTC_module.h"
#include "../../HAL/TWI/TWI_HAL.h"
#include "../../util/bit_operators.h"

//Functions
void test_blink_LED();
static void test_timing();

//Vars
uint16_t value=0;

void test_TC0_HAL_start(){
	
	uart1_hal_init();
	
	while(1){
		value=0;
		//test_blink_LED();
		test_timing();
		
		_delay_ms(2000);
	}
	
}

void callback(){
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
	
	TC0_HAL_init(5, &callback);
	uart1_hal_send_string("Start ");
	
	TC0_HAL_start();
	while(value==0){}
	uart1_hal_send_string("Stop ");
	TC0_HAL_stop();
}


/************************************************************************/
/* Test callback and time                                               */
/************************************************************************/



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