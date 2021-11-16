/*
 * lora_module.h
 *
 * Created: 04-10-2021 17:21:25
 *  Author: Mainframe
 */ 


#ifndef LORA_MODULE_H_
#define LORA_MODULE_H_

#include <xc.h>
#include <stdbool.h>

#define LORA_PORT 100
#define LORA_UPLINK_BLOCK_TIME 30

typedef enum {
	LM_STATUS_SUCCESS,
	LM_STATUS_FATAL_ERROR,
	LM_STATUS_TRY_AGAIN,
	LM_STATUS_CONF_ERR,
	LM_STATUS_MAC_ERR,
	LM_STATUS_INV_DATA_LEN,
}LM_STATUS;

LM_STATUS LM_join_network(char deveui[], char appeui[], char appkey[]);
LM_STATUS LM_send_uplink(uint8_t data[], uint8_t length);
LM_STATUS LM_put_to_sleep();
LM_STATUS LM_wake_up();
bool LM_is_free();
LM_STATUS LM_reset_module();
void LM_forward_msg(const char msg[], char res[]);
void LM_send_break();



#endif /* LORA_MODULE_H_ */