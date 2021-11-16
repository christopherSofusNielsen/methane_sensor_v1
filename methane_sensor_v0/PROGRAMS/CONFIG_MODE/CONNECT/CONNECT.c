/*
 * CONNECT.c
 *
 * Created: 11-11-2021 10:38:50
 *  Author: Mainframe
 */ 

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>


#include "CONNECT.h"
#include "../../../HAL/UART1/UART1_HAL.h"
#include "../../../HAL/UART0/UART0_HAL.h"
#include "../../../HAL/RN2483/RN2483_HAL.h"
#include "../../../MODULES/LORA_module/lora_module.h"
#include "../UTIL/util.h"

extern char cnf_rx_cmd[];
extern char cnf_reply[];

static bool CON_RN2483();
static STATES_CON_LORA CON_RN2483_parse(char msg[]);

bool CONNECT(const char cmd[]){
	char par[20];
	
	if(!get_parameter(cmd, par, 1)) return false;
	
	if(strcmp(par, C_LORA)==0){
		return CON_RN2483();
	}else{
		return false;
	}
}



static bool CON_RN2483(){
	STATES_CON_LORA state=CL_INIT;
	
	while (1)
	{
		switch(state){
			case CL_INIT:
				uart0_hal_init();
				rn2483_init();
				LM_reset_module();
				uart1_hal_send_string("******* You are directly connected to RN2483 ********");
				state=CL_READ_MSG;
			break;
			
			case CL_READ_MSG:
				state=CL_READ_MSG;
				if(uart1_hal_message_ready()){
					uart1_hal_read_message_as_str(cnf_rx_cmd);
					state=CL_PARSE_MSG;
				}
			break;
			
			case CL_PARSE_MSG:
				state=CON_RN2483_parse(cnf_rx_cmd);
			break;
			
			case CL_FORWARD:
				sprintf(cnf_reply, "-->%s", (const char *) cnf_rx_cmd );
				uart1_hal_send_string(cnf_reply);
				LM_forward_msg(cnf_rx_cmd, cnf_reply);
				uart1_hal_send_string(cnf_reply);
				state=CL_READ_MSG;
			break;
			
			case CL_SEND_BREAK:
				sprintf(cnf_reply, "-->%s", "break 0x55" );
				uart1_hal_send_string(cnf_reply);
				_delay_ms(50);
				LM_send_break();
				state=CL_READ_MSG;
			break;
			
			case CL_READ:
				if(uart0_hal_message_ready()){
					uart0_hal_read_message_as_str(cnf_reply);
				}else{
					strcpy(cnf_reply, "null");
				}
				uart1_hal_send_string(cnf_reply);
				state=CL_READ_MSG;
			break;
			
			case CL_EXIT:
				uart0_hal_clear_rx_buffer();
				uart1_hal_send_string("Closing connection to RN2483...");
				return true;
		}
	}
	
	return true;
}

static STATES_CON_LORA CON_RN2483_parse(char msg[]){
	if(strncmp(msg, "exit", strlen("exit"))==0)
	{
		return CL_EXIT;
	}
	else if (strncmp(msg, "break", strlen("break"))==0)
	{
		return CL_SEND_BREAK;
	}
	else if (strncmp(msg, "read", strlen("read"))==0)
	{
		return CL_READ;
	}
	return CL_FORWARD;
}