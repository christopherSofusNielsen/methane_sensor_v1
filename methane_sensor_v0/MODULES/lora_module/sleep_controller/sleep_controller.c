/*
 * sleep_controller.c
 *
 * Created: 12-10-2021 11:24:49
 *  Author: Mainframe
 */ 

#include <stdbool.h>
#include <string.h>
#include "sleep_controller.h"
#include "../RN2483_protocol/RN2483_protocol.h"
#include "../util/util.h"
#include "../../../HAL/UART0/UART0_HAL.h"

LM_STATUS SC_put_to_sleep(){
	
	sys_sleep_cmd(lm_msg, (uint32_t) 86400000);
	attach_ending(lm_msg);
	util_transmit_msg(lm_msg);
	
	//Wait for response
	while(uart0_hal_message_ready()==false){}
	
	util_read_msg(lm_msg);
	
	if(strcmp(lm_msg, OK)==0) return LM_STATUS_SUCCESS;
	return LM_STATUS_FATAL_ERROR;
}

LM_STATUS SC_wake_up(){
	uart0_hal_send_break(0x55);
	
	//Wait for response
	while(uart0_hal_message_ready()==false){}
	
	util_read_msg(lm_msg);
	
	if(strcmp(lm_msg, OK)==0) return LM_STATUS_SUCCESS;
	return LM_STATUS_FATAL_ERROR;
}