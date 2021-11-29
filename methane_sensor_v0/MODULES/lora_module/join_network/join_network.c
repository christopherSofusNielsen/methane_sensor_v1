/*
 * join_network.c
 *
 * Created: 05-10-2021 14:57:07
 *  Author: Christopher S. Nielsen
 */ 
	

#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include "join_network.h"
#include "../lora_module.h"
#include "../../../HAL/RN2483/RN2483_HAL.h"
#include "../RN2483_protocol/RN2483_protocol.h"
#include "../../../HAL/UART0/UART0_HAL.h"
#include "../util/util.h"

static LM_STATUS parse_err(char *msg);

LM_STATUS JN_join_network(char deveui[], char appeui[], char appkey[]){
	
	JN_STATES state=JN_HW_RESET_RN2483;
	LM_STATE_DATA stateData;
	
	while(1){
		switch(state){
			case JN_HW_RESET_RN2483:
				rn2483_reset_active();
				_delay_ms(1000);
				rn2483_reset_deactive();
				
				util_set_pending_states(&stateData, JN_SOFT_RESET_LORA, JN_JOIN_FATAL_ERROR);
				util_set_match_case(&stateData, LM_BEGINS_WITH, RN2483);
				
				state=JN_WAIT_FOR_INCOMMING;
			break;
			
			case JN_SOFT_RESET_LORA:
				cmd_with_ending(lm_msg, MAC_RESET_TO_868_BAND);
				uart0_hal_send_string(lm_msg);
				
				util_set_pending_states(&stateData, JN_LOAD_DEVEUI, JN_JOIN_FATAL_ERROR);
				util_set_match_case(&stateData, LM_EXACT, OK);
				state=JN_WAIT_FOR_INCOMMING;
			break;
			
			case JN_LOAD_DEVEUI:
				mac_set_deveui(lm_msg, deveui);
				attach_ending(lm_msg);
				uart0_hal_send_string(lm_msg);
				
				util_set_pending_states(&stateData, JN_LOAD_APPKEY, JN_JOIN_FATAL_ERROR);
				util_set_match_case(&stateData, LM_EXACT, OK);
				state=JN_WAIT_FOR_INCOMMING;
			break;
			
			case JN_LOAD_APPKEY:
				mac_set_appkey(lm_msg, appkey);
				attach_ending(lm_msg);
				uart0_hal_send_string(lm_msg);
				
				util_set_pending_states(&stateData, JN_LOAD_APPEUI, JN_JOIN_FATAL_ERROR);
				util_set_match_case(&stateData, LM_EXACT, OK);
				state=JN_WAIT_FOR_INCOMMING;
			break;
			
			case JN_LOAD_APPEUI:
				mac_set_appeui(lm_msg, appeui);
				attach_ending(lm_msg);
				uart0_hal_send_string(lm_msg);
				
				util_set_pending_states(&stateData, JN_LOAD_ADR, JN_JOIN_FATAL_ERROR);
				util_set_match_case(&stateData, LM_EXACT, OK);
				state=JN_WAIT_FOR_INCOMMING;
			break;
			
			case JN_LOAD_ADR:
				mac_set_ADR(lm_msg, 1);
				attach_ending(lm_msg);
				uart0_hal_send_string(lm_msg);
				
				util_set_pending_states(&stateData, JN_LOAD_SAVE, JN_JOIN_FATAL_ERROR);
				util_set_match_case(&stateData, LM_EXACT, OK);
				state=JN_WAIT_FOR_INCOMMING;
			break;
			
			case JN_LOAD_SAVE:
				cmd_with_ending(lm_msg, MAC_SAVE);
				uart0_hal_send_string(lm_msg);
				
				util_set_pending_states(&stateData, JN_JOIN_NETWORK, JN_JOIN_FATAL_ERROR);
				util_set_match_case(&stateData, LM_EXACT, OK);
				state=JN_WAIT_FOR_INCOMMING;
			break;
			
			case JN_JOIN_NETWORK:
				cmd_with_ending(lm_msg, MAC_JOIN_OTAA);
				uart0_hal_send_string(lm_msg);
				
				util_set_pending_states(&stateData, JN_JOIN_NETWORK_RES_2, JN_JOIN_NETWORK_FAILED);
				util_set_match_case(&stateData, LM_EXACT, OK);
				state=JN_WAIT_FOR_INCOMMING;
			break;
			
			case JN_JOIN_NETWORK_RES_2:
				util_set_pending_states(&stateData, JN_JOIN_SUCCESS, JN_JOIN_NETWORK_FAILED);
				util_set_match_case(&stateData, LM_EXACT, ACCEPTED);
				state=JN_WAIT_FOR_INCOMMING;
			break;
				
			case JN_JOIN_SUCCESS:
				return LM_STATUS_SUCCESS;
				
			case JN_JOIN_FATAL_ERROR:
				return LM_STATUS_FATAL_ERROR;
				
			case JN_JOIN_NETWORK_FAILED:
				return parse_err(lm_msg);
				
				
			case JN_WAIT_FOR_INCOMMING:
				if(uart0_hal_message_ready()){
					uart0_hal_read_message_as_str(lm_msg);
					state=util_match_message(&stateData, lm_msg);
				}
			break;
		}
	}
	return LM_STATUS_FATAL_ERROR;
}


LM_STATUS parse_err(char *msg){
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
