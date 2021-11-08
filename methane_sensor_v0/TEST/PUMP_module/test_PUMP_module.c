/*
 * test_PUMP_module.c
 *
 * Created: 08-11-2021 16:38:21
 *  Author: Mainframe
 */ 
#include <xc.h>
#include <util/delay.h>

#include "test_PUMP_module.h"
#include "../../HAL/UART0/UART0_HAL.h"
#include "../../HAL/TC2/TC2_HAL.h"
#include "../../MODULES/PUMP_module/PUMP_module.h"


static void test_pump_start();


void test_PUMP_module_start(){
	uart0_hal_init();
	TC2_HAL_init();
	
	while(1){
		test_pump_start();
		_delay_ms(2000);
	}
}

static void test_pump_start(){
	uart0_hal_send_string("Start");
	PUMP_start(30);
	while(!PUMP_is_done()){}
	uart0_hal_send_string("Stop");
}