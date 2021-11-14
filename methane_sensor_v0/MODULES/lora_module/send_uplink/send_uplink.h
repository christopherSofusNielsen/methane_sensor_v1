/*
 * send_uplink.h
 *
 * Created: 11-10-2021 16:01:12
 *  Author: Mainframe
 */ 


#ifndef SEND_UPLINK_H_
#define SEND_UPLINK_H_

#include <xc.h>
#include "../lora_module.h"

typedef enum {
	SU_SEND_PAYLOAD,
	SU_SEND_PAYLOAD_RES_2,
	SU_TX_DECODE_RES,
	SU_SENDING_SUCCESS,
	SU_WAIT_FOR_INCOMMING,
}SU_STATES;

LM_STATUS SU_send_uplink(uint8_t port, uint8_t data[], uint8_t length);


#endif /* SEND-UPLINK_H_ */