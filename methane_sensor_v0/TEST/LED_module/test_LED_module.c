/*
 * test_LED_module.c
 *
 * Created: 08-11-2021 21:55:17
 *  Author: Mainframe
 */ 
#include <xc.h>
#include <util/delay.h>
#include <stdbool.h>

#include "test_LED_module.h"
#include "../../HAL/UART0/UART0_HAL.h"
#include "../../HAL/PM/PM_HAL.h"
#include "../../HAL/TC2/TC2_HAL.h"
#include "../../MODULES/LED_module/LED_module.h"

static void test_try_join();
static void test_sequences();

void test_LED_module_start(){
	uart0_hal_init();
	PM_HAL_LED_power_init();
	TC2_HAL_init();
	
	
	while(1){
		//test_try_join();
		//test_sequences();
		_delay_ms(2000);
	}
}

static void test_try_join(){
	LED_start_try_join();
	_delay_ms(6000);
	LED_stop_try_join();
}

static void test_sequences(){
	LED_join_success();
	_delay_ms(1000);
	LED_join_denied();
	_delay_ms(1000);
	LED_conf_err();
	_delay_ms(1000);
	LED_fatal_err();
}