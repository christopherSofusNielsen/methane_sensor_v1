/*
 * join_network.h
 *
 * Created: 05-10-2021 14:57:21
 *  Author: Christopher S. Nielsen
 */ 


#ifndef JOIN_NETWORK_H_
#define JOIN_NETWORK_H_

#include "../lora_module.h"

typedef enum {
	JN_HW_RESET_RN2483,
	JN_WAIT_FOR_INCOMMING,
	JN_SOFT_RESET_LORA,
	JN_LOAD_DEVEUI,
	JN_LOAD_APPKEY,
	JN_LOAD_APPEUI,
	JN_LOAD_ADR,
	JN_LOAD_SAVE,
	JN_JOIN_NETWORK,
	JN_JOIN_NETWORK_RES_2,
	JN_JOIN_NETWORK_FAILED,
	JN_JOIN_SUCCESS,
	JN_JOIN_FATAL_ERROR
}JN_STATES;

LM_STATUS JN_join_network(char deveui[], char appeui[], char appkey[]);



#endif /* JOIN_NETWORK_H_ */