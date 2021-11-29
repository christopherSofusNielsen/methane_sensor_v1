/*
 * util.c
 *
 * Created: 05-10-2021 16:03:19
 *  Author: Christopher S. Nielsen
 */ 

#include <string.h>
#include <stdbool.h>
#include <util/delay.h>

#include "util.h"
#include "../lora_module.h"
#include "../../../HAL/UART0/UART0_HAL.h"
#include "../../../HAL/RN2483/RN2483_HAL.h"
#include "../RN2483_protocol/RN2483_protocol.h"
#include "../../../HAL/TC2/TC2_HAL.h"

static uint8_t wd_cnt=0;
static int8_t wd_cb_pointer;
char lm_msg[200];

static void cb_watchdog_counter();

void util_set_pending_states(LM_STATE_DATA *sd, int success, int failed){
	sd->success_state=success;
	sd->failure_state=failed;
}

void util_set_match_case(LM_STATE_DATA *sd, LM_MATCH_CASES matchCase, const char *matchString){
	sd->matchCase=matchCase;
	sd->matchString=matchString;
}

int util_match_message(LM_STATE_DATA *sd, char *msg){
	switch(sd->matchCase){
		case LM_EXACT:
		return strcmp(msg, sd->matchString)==0?sd->success_state:sd->failure_state;

		case LM_BEGINS_WITH:
		return util_str_begins_with(msg, sd->matchString)?sd->success_state:sd->failure_state;

		case LM_NO_TEST:
		return sd->success_state;
		default:
		return sd->failure_state;
	}
}


bool util_str_begins_with(const char *s1, const char *s2){
	return strncmp(s1, s2, strlen(s2))==0?true:false;
}

LM_STATUS util_reset_module(){
	rn2483_reset_active();
	_delay_ms(1000);
	rn2483_reset_deactive();
	
	while(!uart0_hal_message_ready()){};
	
	uart0_hal_read_message_as_str(lm_msg);
	
	if(util_str_begins_with(lm_msg, RN2483)){
		return LM_STATUS_SUCCESS;
	}else{
		return LM_STATUS_FATAL_ERROR;
	}
}

void util_lora_forward_msg(const char msg[], char res[]){
	strcpy(lm_msg, msg);
	attach_ending(lm_msg);
	uart0_hal_send_string(lm_msg);
	
	wd_cnt=0;
	wd_cb_pointer=TC2_HAL_subscribe(&cb_watchdog_counter);
	
	while(!uart0_hal_message_ready() && wd_cnt<15);
	TC2_HAL_cancel(wd_cb_pointer);
	if(wd_cnt>=15){
		strcpy(res, "no reply...");
		uart0_hal_read_message_as_str(res);
	}else{
		uart0_hal_read_message_as_str(res);
	}
}

void util_send_break(){
	uart0_hal_send_break(0x55);
}

static void cb_watchdog_counter(){
	wd_cnt++;
}