/*
 * test_UART0_HAL.c
 *
 * Created: 04-11-2021 10:59:37
 *  Author: Christopher S. Nielsen
 */ 

#include <xc.h>
#include <util/delay.h>

#include "test_UART0_HAL.h"
#include "../../util/bit_operators.h"
#include "../../HAL/UART0/UART0_HAL.h"
#include "../../HAL/UART1/UART1_HAL.h"



void test_UART0_HAL_start(){
	uart0_hal_init();
	uart1_hal_init();
	
	uart1_hal_send_string("Running ");
	while(1){
		if(uart0_hal_message_ready()){
			uart1_hal_send_string("MSG received");
		}
		_delay_ms(1000);
	}
}