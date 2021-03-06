/*
 * CONNECT.h
 *
 * Created: 11-11-2021 10:38:58
 *  Author: Christopher S. Nielsen
 */ 


#ifndef CONNECT_H_
#define CONNECT_H_

#include <stdbool.h>

#define C_LORA "lora"

typedef enum{
	CL_INIT,
	CL_READ_MSG,
	CL_PARSE_MSG,
	CL_FORWARD,
	CL_SEND_BREAK,
	CL_READ,
	CL_EXIT
}STATES_CON_LORA;


bool CONNECT(const char cmd[]);



#endif /* CONNECT_H_ */