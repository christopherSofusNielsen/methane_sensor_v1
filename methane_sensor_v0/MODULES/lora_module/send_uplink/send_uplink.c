/*
 * send_uplink.c
 *
 * Created: 11-10-2021 16:00:55
 *  Author: Mainframe
 */ 

#include <string.h>
#include <stdbool.h>
#include "send_uplink.h"
#include "../util/util.h"
#include "../lora_module.h"
#include "../../../HAL/UART0/UART0_HAL.h"
#include "../RN2483_protocol/RN2483_protocol.h"

static SU_STATES decode_tx_response(const char msg[]);
static bool beginsWidth(const char *s1, const char *s2);


LM_STATUS SU_send_uplink(uint8_t port, uint8_t data[], uint8_t length){
	
	SU_STATES state=SU_SEND_PAYLOAD;
	LM_STATE_DATA stateData;
	
	while(1){
		switch(state){
			case SU_SEND_PAYLOAD:
				mac_tx_confirmed(lm_msg, port, data, length);
				attach_ending(lm_msg);
				util_transmit_msg(lm_msg);
				
				util_setPendingStates(&stateData, SU_SEND_PAYLOAD_RES_2, SU_TX_DECODE_RES);
				util_setMatchCase(&stateData, LM_EXACT, OK);
				state=SU_WAIT_FOR_INCOMMING;
			break;
			
			case SU_SEND_PAYLOAD_RES_2:
				util_setPendingStates(&stateData, SU_SENDING_SUCCESS, SU_TX_DECODE_RES);
				util_setMatchCase(&stateData, LM_EXACT, MAC_TX_OK);
				state=SU_WAIT_FOR_INCOMMING;
			break;
			
			case SU_TX_DECODE_RES:
				state=decode_tx_response(lm_msg);
			break;
			
			case SU_HANDLE_DOWNLINK:
				return LM_STATUS_SUCCESS;
			
			case SU_SENDING_SUCCESS:
				return LM_STATUS_SUCCESS;
				
			case SU_TRY_AGAIN:
				return LM_STATUS_TRY_AGAIN;
			
			case SU_NO_ACK:
				return LM_STATUS_MAC_ERR;
				
			case SU_INV_DATA_LEN:
				return LM_STATUS_INV_DATA_LEN;
			
			case SU_FATAL_ERROR:
				return LM_STATUS_FATAL_ERROR;	
			
			case SU_WAIT_FOR_INCOMMING:
				if(uart0_hal_message_ready()){
					util_read_msg(lm_msg);
					state=util_matchMessage(&stateData, lm_msg);
				}
			break;
		}
	}
}

static SU_STATES decode_tx_response(const char msg[]){
	if(strcmp(msg, NO_FREE_CH)==0)return SU_TRY_AGAIN;
	if(strcmp(msg, MAC_ERR)==0)return SU_NO_ACK;
	if(beginsWidth(msg, MAC_RX)) return SU_HANDLE_DOWNLINK;
	if(beginsWidth(msg, INVALID_DATA_LEN)) return SU_INV_DATA_LEN;
	
	return SU_FATAL_ERROR;
}

static bool beginsWidth(const char *s1, const char *s2){
	return strncmp(s1, s2, strlen(s2))==0?true:false;
}