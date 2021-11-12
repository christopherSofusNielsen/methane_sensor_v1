/*
 * LED_module.c
 *
 * Created: 08-11-2021 21:53:53
 *  Author: Mainframe
 */ 
#include <util/delay.h>
#include <xc.h>

#include "LED_module.h"
#include "../../HAL/PM/PM_HAL.h"
#include "../../HAL/TC2/TC2_HAL.h"

static void cb_blink();
static void err_code(uint8_t times);
static void blink(uint8_t times);


static int8_t cbPointer;

void LED_start_try_join(){
	cbPointer=TC2_HAL_subscribe(&cb_blink);
}

void LED_stop_try_join(){
	TC2_HAL_cancel(cbPointer);
	PM_HAL_LED_power(false);
}

void LED_join_success(){
	for (uint8_t i=0; i<10; i++)
	{
		PM_HAL_LED_power(true);
		_delay_ms(100);
		PM_HAL_LED_power(false);
		_delay_ms(100);
	}
}

void LED_join_denied(){
	err_code(3);
}

void LED_conf_err(){
	err_code(5);
}

void LED_fatal_err(){
	err_code(7);
}

static void cb_blink(){
	PM_HAL_LED_toggle();
}


static void err_code(uint8_t times){
	PM_HAL_LED_power(true);
	_delay_ms(2000);
	blink(times);
	_delay_ms(1000);
}

static void blink(uint8_t times){
	for (uint8_t i=0; i<times; i++)
	{
		PM_HAL_LED_power(false);
		_delay_ms(300);
		PM_HAL_LED_power(true);
		_delay_ms(300);
	}
	PM_HAL_LED_power(false);
}

