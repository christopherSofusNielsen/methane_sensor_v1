/*
 * util.c
 *
 * Created: 05-10-2021 16:03:19
 *  Author: Mainframe
 */ 

#include <string.h>
#include <stdbool.h>
#include <util/delay.h>

#include "util.h"
#include "../lora_module.h"
#include "../../../HAL/UART0/UART0_HAL.h"
#include "../../../HAL/RN2483/RN2483_HAL.h"
#include "../RN2483_protocol/RN2483_protocol.h"


char lm_msg[200];

void util_setPendingStates(LM_STATE_DATA *sd, int success, int failed){
	sd->success_state=success;
	sd->failure_state=failed;
}

void util_setMatchCase(LM_STATE_DATA *sd, LM_MATCH_CASES matchCase, const char *matchString){
	sd->matchCase=matchCase;
	sd->matchString=matchString;
}

int util_matchMessage(LM_STATE_DATA *sd, char *msg){
	switch(sd->matchCase){
		case LM_EXACT:
		return strcmp(msg, sd->matchString)==0?sd->success_state:sd->failure_state;

		case LM_BEGINS_WITH:
		return util_strbeginswith(msg, sd->matchString)?sd->success_state:sd->failure_state;

		case LM_NO_TEST:
		return sd->success_state;
		default:
		return sd->failure_state;
	}
}

void util_transmit_msg(char msg[]){
	uart0_hal_send_message((uint8_t*) msg, strlen(msg));
}

void util_read_msg(char msg[]){
	uart0_hal_read_message_as_str(msg);
}

LM_STATUS util_parse_err(char *msg){
	if(strcmp(msg, NO_FREE_CH)==0){
		return LM_STATUS_TRY_AGAIN;
	}
	
	if(strcmp(msg, DENIED)==0){
		return LM_STATUS_TRY_AGAIN;
	}
	
	if(strcmp(msg, KEYS_NOT_INIT)==0){
		return LM_STATUS_CONF_ERR;
	}
	
	return LM_STATUS_FATAL_ERROR;
}

bool util_strbeginswith(char *s1, const char *s2){
	return strncmp(s1, s2, strlen(s2))==0?true:false;
}

LM_STATUS util_reset_module(){
	rn2483_reset_active();
	_delay_ms(1000);
	rn2483_reset_deactive();
	
	while(!uart0_hal_message_ready()){};
	
	util_read_msg(lm_msg);
	
	if(util_strbeginswith(lm_msg, RN2483)){
		return LM_STATUS_SUCCESS;
	}else{
		return LM_STATUS_ERROR;
	}
}

void util_lora_forward_msg(const char msg[], char res[]){
	strcpy(lm_msg, msg);
	attach_ending(lm_msg);
	util_transmit_msg(lm_msg);
	
	while(!uart0_hal_message_ready());
	util_read_msg(res);
}