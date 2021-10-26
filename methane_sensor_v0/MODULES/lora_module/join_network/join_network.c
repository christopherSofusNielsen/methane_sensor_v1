/*
 * join_network.c
 *
 * Created: 05-10-2021 14:57:07
 *  Author: Mainframe
 */ 
#define F_CPU 8000000UL	

#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include "join_network.h"
#include "../lora_module.h"
#include "../../../HAL/RN2483/RN2483_HAL.h"
#include "../RN2483_protocol/RN2483_protocol.h"
#include "../../../HAL/UART0/UART0_HAL.h"
#include "../util/util.h"

LM_STATUS JN_join_network(){
	
	JN_STATES state=JN_HW_RESET_RN2483;
	LM_STATE_DATA stateData;
	char parameters[33];
	
	
	
	while(1){
		switch(state){
			case JN_HW_RESET_RN2483:
				rn2483_reset_active();
				_delay_ms(1000);
				rn2483_reset_deactive();
				
				util_setPendingStates(&stateData, JN_SOFT_RESET_LORA, JN_JOIN_FATAL_ERROR);
				util_setMatchCase(&stateData, LM_BEGINS_WITH, RN2483);
				
				state=JN_WAIT_FOR_INCOMMING;
			break;
			
			case JN_SOFT_RESET_LORA:
				cmd_with_ending(lm_msg, MAC_RESET_TO_868_BAND);
				util_transmit_msg(lm_msg);
				
				util_setPendingStates(&stateData, JN_LOAD_DEVEUI, JN_JOIN_FATAL_ERROR);
				util_setMatchCase(&stateData, LM_EXACT, OK);
				state=JN_WAIT_FOR_INCOMMING;
			break;
			
			case JN_LOAD_DEVEUI:
				util_get_deveui_as_string(parameters);
				mac_set_deveui(lm_msg, parameters);
				attach_ending(lm_msg);
				util_transmit_msg(lm_msg);
				
				util_setPendingStates(&stateData, JN_LOAD_APPKEY, JN_JOIN_FATAL_ERROR);
				util_setMatchCase(&stateData, LM_EXACT, OK);
				state=JN_WAIT_FOR_INCOMMING;
			break;
			
			case JN_LOAD_APPKEY:
				util_get_appkey_as_string(parameters);
				mac_set_appkey(lm_msg, parameters);
				attach_ending(lm_msg);
				util_transmit_msg(lm_msg);
				
				util_setPendingStates(&stateData, JN_LOAD_APPEUI, JN_JOIN_FATAL_ERROR);
				util_setMatchCase(&stateData, LM_EXACT, OK);
				state=JN_WAIT_FOR_INCOMMING;
			break;
			
			case JN_LOAD_APPEUI:
				util_get_appeui_as_string(parameters);
				mac_set_appeui(lm_msg, parameters);
				attach_ending(lm_msg);
				util_transmit_msg(lm_msg);
				
				util_setPendingStates(&stateData, JN_LOAD_ADR, JN_JOIN_FATAL_ERROR);
				util_setMatchCase(&stateData, LM_EXACT, OK);
				state=JN_WAIT_FOR_INCOMMING;
			break;
			
			case JN_LOAD_ADR:
				mac_set_ADR(lm_msg, 1);
				attach_ending(lm_msg);
				util_transmit_msg(lm_msg);
				
				util_setPendingStates(&stateData, JN_LOAD_SAVE, JN_JOIN_FATAL_ERROR);
				util_setMatchCase(&stateData, LM_EXACT, OK);
				state=JN_WAIT_FOR_INCOMMING;
			break;
			
			case JN_LOAD_SAVE:
				cmd_with_ending(lm_msg, MAC_SAVE);
				util_transmit_msg(lm_msg);
				
				util_setPendingStates(&stateData, JN_JOIN_NETWORK, JN_JOIN_FATAL_ERROR);
				util_setMatchCase(&stateData, LM_EXACT, OK);
				state=JN_WAIT_FOR_INCOMMING;
			break;
			
			case JN_JOIN_NETWORK:
				cmd_with_ending(lm_msg, MAC_JOIN_OTAA);
				util_transmit_msg(lm_msg);
				
				util_setPendingStates(&stateData, JN_JOIN_NETWORK_RES_2, JN_JOIN_NETWORK_FAILED);
				util_setMatchCase(&stateData, LM_EXACT, OK);
				state=JN_WAIT_FOR_INCOMMING;
			break;
			
			case JN_JOIN_NETWORK_RES_2:
				util_setPendingStates(&stateData, JN_JOIN_SUCCESS, JN_JOIN_NETWORK_FAILED);
				util_setMatchCase(&stateData, LM_EXACT, ACCEPTED);
				state=JN_WAIT_FOR_INCOMMING;
			break;
				
			case JN_JOIN_SUCCESS:
				return LM_STATUS_SUCCESS;
				
			case JN_JOIN_FATAL_ERROR:
				return LM_STATUS_FATAL_ERROR;
				
			case JN_JOIN_NETWORK_FAILED:
				return LM_STATUS_TRY_AGAIN;
				
				
			case JN_WAIT_FOR_INCOMMING:
				if(uart0_hal_message_ready()){
					util_read_msg(lm_msg);
					state=util_matchMessage(&stateData, lm_msg);
				}
			break;
		}
	}
	return LM_STATUS_FATAL_ERROR;
}