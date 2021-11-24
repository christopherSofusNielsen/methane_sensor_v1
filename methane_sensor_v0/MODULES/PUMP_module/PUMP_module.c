/*
 * PUMP_module.c
 *
 * Created: 08-11-2021 16:35:54
 *  Author: Christopher S. Nielsen
 */ 
#include "PUMP_module.h"
#include "../../HAL/TC2/TC2_HAL.h"
#include "../../HAL/PM/PM_HAL.h"

#include <xc.h>

static uint16_t _seconds=0;
static uint16_t cnt=0;
static int8_t eventPointer;

static void cb_cnt_time();


void PUMP_start(uint16_t seconds){
	cnt=0;
	_seconds=seconds;
	eventPointer=TC2_HAL_subscribe(&cb_cnt_time);
	PM_HAL_BC_power(true);
}

bool PUMP_is_done(){
	return cnt>=_seconds;
}

static void cb_cnt_time(){
	cnt++;
	if(cnt>=_seconds){
		TC2_HAL_cancel(eventPointer);
		PM_HAL_BC_power(false);
	}
}

