/*
 * lora_module.h
 *
 * Created: 04-10-2021 17:21:25
 *  Author: Mainframe
 */ 


#ifndef LORA_MODULE_H_
#define LORA_MODULE_H_

#include <xc.h>

#define LORA_PORT 100

typedef enum {
	LM_STATUS_SUCCESS,
	LM_STATUS_ERROR,
	LM_STATUS_FATAL_ERROR,
	LM_STATUS_TRY_AGAIN
}LM_STATUS;

LM_STATUS LM_join_network();
LM_STATUS LM_send_uplink(uint8_t data[], uint8_t length);
LM_STATUS LM_put_to_sleep();
LM_STATUS LM_wake_up();



#endif /* LORA_MODULE_H_ */