/*
 * test_sleep_hall.c
 *
 * Created: 18-10-2021 09:55:26
 *  Author: Christopher S. Nielsen
 */ 
#define F_CPU 8000000UL	
#include <xc.h>
#include <util/delay.h>
#include <string.h>

#include "test_PM_HAL.h"
#include "../../HAL/PM/PM_HAL.h"
#include "../../HAL/UART1/UART1_HAL.h"

void test_sleep_cmd();

void test_PM_HAL_start(){
	uart1_hal_init();
	uart1_hal_send_string("Init ");
	_delay_ms(500);
	while(1){
		test_sleep_cmd();
		
	}
}

void test_sleep_cmd(){
	
	uart1_hal_send_string("Before sleep");
	_delay_ms(5000);
	PM_HAL_enter_power_down();
	uart1_hal_send_string("After sleep");
	_delay_ms(200);
}