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

static LM_STATUS decode_tx_response(const char msg[]);

LM_STATUS SU_send_uplink(uint8_t port, uint8_t data[], uint8_t length){
	
	SU_STATES state=SU_SEND_PAYLOAD;
	LM_STATE_DATA stateData;
	
	while(1){
		switch(state){
			case SU_SEND_PAYLOAD:
				mac_tx_confirmed(lm_msg, port, data, length);
				attach_ending(lm_msg);
				uart0_hal_send_string(lm_msg);
				
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
				return decode_tx_response(lm_msg);
			break;
			
			case SU_SENDING_SUCCESS:
				return LM_STATUS_SUCCESS;
				
			case SU_WAIT_FOR_INCOMMING:
				if(uart0_hal_message_ready()){
					uart0_hal_read_message_as_str(lm_msg);
					state=util_matchMessage(&stateData, lm_msg);
				}
			break;
		}
	}
}

static LM_STATUS decode_tx_response(const char msg[]){
	if(strcmp(msg, NO_FREE_CH)==0)return LM_STATUS_TRY_AGAIN;
	if(strcmp(msg, MAC_ERR)==0)return LM_STATUS_MAC_ERR;
	if(util_strbeginswith(msg, MAC_RX)) return LM_STATUS_SUCCESS;
	if(util_strbeginswith(msg, INVALID_DATA_LEN)) return LM_STATUS_INV_DATA_LEN;
	
	return LM_STATUS_FATAL_ERROR;
}

