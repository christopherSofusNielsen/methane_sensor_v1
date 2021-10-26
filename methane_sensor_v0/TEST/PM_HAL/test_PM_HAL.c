/*
 * test_sleep_hall.c
 *
 * Created: 18-10-2021 09:55:26
 *  Author: Mainframe
 */ 
#define F_CPU 8000000UL	
#include <xc.h>
#include <util/delay.h>
#include <string.h>

#include "test_PM_HAL.h"
#include "../../HAL/PM/PM_HAL.h"
#include "../../HAL/UART0/UART0_HAL.h"

void test_sleep_cmd();

void test_PM_HAL_start(){
	uart0_hal_init();
	char msg[]="Init";
	uart0_hal_send_message(msg, strlen(msg));
	_delay_ms(500);
	while(1){
		test_sleep_cmd();
		
	}
}

void test_sleep_cmd(){
	char msg1[]="Before sleep";
	uart0_hal_send_message(msg1, strlen(msg1));
	_delay_ms(200);
	PM_HAL_enter_power_down();
	char msg2[]="After sleep";
	uart0_hal_send_message(msg2, strlen(msg2));
	_delay_ms(200);
}