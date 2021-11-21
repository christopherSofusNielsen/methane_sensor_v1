/*
 * test_PUMP_module.c
 *
 * Created: 08-11-2021 16:38:21
 *  Author: Mainframe
 */ 
#include <xc.h>
#include <util/delay.h>

#include "test_PUMP_module.h"
#include "../../HAL/UART1/UART1_HAL.h"
#include "../../HAL/TC2/TC2_HAL.h"
#include "../../MODULES/PUMP_module/PUMP_module.h"
#include "../../HAL/PM/PM_HAL.h"


static void test_pump_start();
static void power_test();


void test_PUMP_module_start(){
	uart1_hal_init();
	TC2_HAL_init();
	PM_HAL_BC_power_init();
	
	while(1){
		//test_pump_start();
		power_test();
		_delay_ms(2000);
	}
}

static void test_pump_start(){
	uart1_hal_send_string("Start");
	PUMP_start(30);
	while(!PUMP_is_done()){}
	uart1_hal_send_string("Stop");
}

static void power_test(){
	uart1_hal_send_string("Start PUMP power test");
	PUMP_start(5);
	while(!PUMP_is_done()){}
	uart1_hal_send_string("Done, power off");
	_delay_ms(2000);
	uart1_hal_send_string("End PUMP power test");
}