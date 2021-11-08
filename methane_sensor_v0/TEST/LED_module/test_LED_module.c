/*
 * test_LED_module.c
 *
 * Created: 08-11-2021 21:55:17
 *  Author: Mainframe
 */ 
#include <xc.h>
#include <util/delay.h>

#include "test_LED_module.h"
#include "../../HAL/UART0/UART0_HAL.h"


void test_LED_module_start(){
	uart0_hal_init();
	
	while(1){
		uart0_hal_send_string("Working ");
		_delay_ms(2000);
	}
}