/*
 * sleep_controller.c
 *
 * Created: 12-10-2021 11:24:49
 *  Author: Mainframe
 */ 

#define LM_SLEEP_WATCHDOG_CNT 10

#include <xc.h>
#include <stdbool.h>
#include <string.h>
#include <util/delay.h>
#include "sleep_controller.h"
#include "../RN2483_protocol/RN2483_protocol.h"
#include "../util/util.h"
#include "../../../HAL/UART0/UART0_HAL.h"
#include "../../../HAL/TC2/TC2_HAL.h"

static void cb_counter();

static uint16_t cnt=0;
static int8_t eventPointer;

LM_STATUS SC_put_to_sleep(){
	
	sys_sleep_cmd(lm_msg, (uint32_t) 86400000);
	attach_ending(lm_msg);
	uart0_hal_send_string(lm_msg);
	_delay_ms(200);
	return LM_STATUS_SUCCESS;
}

LM_STATUS SC_wake_up(){
	uart0_hal_send_break(0x55);
	
	//Create watchdog
	cnt=0;
	eventPointer=TC2_HAL_subscribe(&cb_counter);
	
	//Expect ok from sleep
	while(uart0_hal_message_ready()==false && cnt<LM_SLEEP_WATCHDOG_CNT){}
		
	TC2_HAL_cancel(eventPointer);
		
	if(cnt>=LM_SLEEP_WATCHDOG_CNT){
		uart0_hal_clear_rx_buffer();
		return LM_STATUS_FATAL_ERROR;
	}else{
		uart0_hal_read_message_as_str(lm_msg);
		
		if(strcmp(lm_msg, OK)==0) return LM_STATUS_SUCCESS;
		return LM_STATUS_FATAL_ERROR;
	}
}

static void cb_counter(){
	cnt++;
}