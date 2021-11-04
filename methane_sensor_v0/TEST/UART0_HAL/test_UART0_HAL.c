/*
 * test_UART0_HAL.c
 *
 * Created: 04-11-2021 10:59:37
 *  Author: Mainframe
 */ 

#include <xc.h>
#include <util/delay.h>

#include "test_UART0_HAL.h"
#include "../../util/bit_operators.h"
#include "../../HAL/UART0/UART0_HAL.h"



void test_UART0_HAL_start(){
	uart0_hal_init();
	
	
	while(1){
		uart0_hal_send_string("Hey");	
		_delay_ms(1000);
	}
}