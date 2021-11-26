/*
 * lora_module.h
 *
 * Created: 04-10-2021 17:21:25
 *  Author: Christopher S. Nielsen
 */ 


#ifndef LORA_MODULE_H_
#define LORA_MODULE_H_

#include <xc.h>
#include <stdbool.h>

//LoRa port used for uplinks
#define LORA_PORT 100

//Block time in seconds between uplinks try
#define LORA_UPLINK_BLOCK_TIME 60

/************************************************************************/
/* Status Type                                                          */
/************************************************************************/
typedef enum {
	LM_STATUS_SUCCESS,
	LM_STATUS_FATAL_ERROR,
	LM_STATUS_TRY_AGAIN,
	LM_STATUS_CONF_ERR,
	LM_STATUS_MAC_ERR,
	LM_STATUS_INV_DATA_LEN,
}LM_STATUS;

/************************************************************************
* Tries to join LoRa-WAN network
* -> char deveui[]: DEVEUI
* -> char appeui[]: APPEUI
* -> char appkey[]: APPKEY
* -> Returns LM_STATUS_SUCCESS when succesfull, otherwise
*	 LM_STATUS_TRY_AGAIN if no free channels or denied
*	 LM_STATUS_CONF_ERR if keys not configured correct (DEVEUI, APPEUI, APPKEY)
*	 LM_STATUS_FATAL_ERROR on other errors
************************************************************************/
LM_STATUS LM_join_network(char deveui[], char appeui[], char appkey[]);

/************************************************************************
* Transmits payload
* -> uint8_t data[]: application payload
* -> uint8_t length: length of payload
* -> Returns LM_STATUS_SUCCESS when succesfull (even if not confirmed), otherwise
*	 LM_STATUS_TRY_AGAIN if no free channels
*	 LM_STATUS_MAC_ERR on mac error
*	 LM_STATUS_INV_DATA_LEN on to big payload for data rate
*	 LM_STATUS_FATAL_ERROR on other errors
************************************************************************/
LM_STATUS LM_send_uplink(uint8_t data[], uint8_t length);

/************************************************************************
* Puts RN2483 module into sleep mode
* -> Returns LM_STATUS_SUCCESS
************************************************************************/
LM_STATUS LM_put_to_sleep();

/************************************************************************
* Wakes up the RN2483 module
* -> Returns LM_STATUS_SUCCESS when successfull, otherwise LM_STATUS_FATAL_ERROR
************************************************************************/
LM_STATUS LM_wake_up();

/************************************************************************/
/* Returns true if free to transmit uplink                              */
/************************************************************************/
bool LM_is_free();

/************************************************************************/
/* Hard reset RN2483                                                    */
/************************************************************************/
LM_STATUS LM_reset_module();

/************************************************************************
* Forwards msg as a string (\0 at the end) to RN2483
* -> const char msg[]: String to forward
* -> char res[]: pointer to where to store the response 
************************************************************************/
void LM_forward_msg(const char msg[], char res[]);

/************************************************************************/
/* Sends break condition to RN2483 module                               */
/************************************************************************/
void LM_send_break();



#endif /* LORA_MODULE_H_ */