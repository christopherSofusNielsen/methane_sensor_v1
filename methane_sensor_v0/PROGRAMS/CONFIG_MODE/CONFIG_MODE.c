/*
 * CONFIG_MODE.c
 *
 * Created: 09-11-2021 13:35:14
 *  Author: Mainframe
 */ 
#include <string.h>

#include "CONFIG_MODE.h"
#include "../../HAL/UART1/UART1_HAL.h"
#include "RW/RW.h"
#include "CONNECT/CONNECT.h"
#include "SAMPLE/SAMPLE.h"

char cnf_rx_cmd[200];
char cnf_reply[200];

static CONF_STATES parse_cmd(char msg[]);
static bool validate_cmd(char msg[]);

void CONF_enter(){
	uart1_hal_init();
	CONF_STATES state=CONF_INTRO;
	
	while(1){
		switch(state){
			case CONF_INTRO:
				uart1_hal_send_string("******** Methane Sensor v1.0 ********");
				uart1_hal_send_string("Setting modules up... Please wait");
				
				if(!init_methane_SCD30()){
					uart1_hal_send_string("Failed to initialize system...");
					state=CONF_EXIT;
				}else{
					state=CONF_CLEAR_FOR_NEW_CMD;
				}
					//state=CONF_CLEAR_FOR_NEW_CMD;
					
			break;
			
			case CONF_WAIT_FOR_CMD:
				state=CONF_WAIT_FOR_CMD;
				if(uart1_hal_message_ready()){
					state=CONF_PARSE;
				}
				if(uart1_hal_rx_full()){
					state=CONF_NOT_VALID;
				}
			break;
			
			case CONF_PARSE:
				uart1_hal_read_message_as_str(cnf_rx_cmd);
				if(validate_cmd(cnf_rx_cmd)){
					state=parse_cmd(cnf_rx_cmd);
				}else{
					state=CONF_FORMAT_ERR;
				}
			break;
			
			case CONF_READ:
				if(handle_read(cnf_rx_cmd, cnf_reply)){
					state=CONF_REPLY;
				}else{
					state=CONF_NOT_VALID;
				}
			break;
			
			case CONF_WRTIE:
				if(handle_write(cnf_rx_cmd, cnf_reply)){
					state=CONF_REPLY;
				}else{
					state=CONF_NOT_VALID;
				}
			break;
			
			case CONF_CONNECT:
				if(CONNECT(cnf_rx_cmd)){
					state=CONF_CLEAR_FOR_NEW_CMD;
				}else{
					state=CONF_NOT_VALID;
				}
			break;
			
			case CONF_SAMPLE:
				if(handle_sample(cnf_rx_cmd, cnf_reply)){
					state=CONF_REPLY;
				}else{
					state=CONF_NOT_VALID;
				}
			break;
			
			case CONF_REPLY:
				uart1_hal_send_string(cnf_reply);
				state=CONF_CLEAR_FOR_NEW_CMD;
			break;
			
			case CONF_NOT_VALID:
				uart1_hal_send_string("Not valid");
				state=CONF_CLEAR_FOR_NEW_CMD;
			break;
			
			case CONF_FORMAT_ERR:
				uart1_hal_send_string("Wrong format");
				state=CONF_CLEAR_FOR_NEW_CMD;
			break;
			
			case CONF_CLEAR_FOR_NEW_CMD:
				uart1_hal_clear_rx_buffer();
				uart1_hal_send_string("******* Ready for new command *******");
				state=CONF_WAIT_FOR_CMD;
			break;
			
			case CONF_EXIT:
				uart1_hal_send_string("Exit...");
				return;
		}
	}
}

static CONF_STATES parse_cmd(char msg[]){
	if(msg[0]=='r') return CONF_READ;
	if(msg[0]=='w') return CONF_WRTIE;
	if(msg[0]=='c') return CONF_CONNECT;
	if(msg[0]=='s') return CONF_SAMPLE;
	if(msg[0]=='q') return CONF_EXIT;
	
	return CONF_NOT_VALID;
}

static bool validate_cmd(char msg[]){
	uint16_t len=strlen(msg);
	bool lastCharWasSpace=false;
	
	if(msg[len-1]==' ') return false;
	
	//Check for multiple space in row
	for (uint16_t i=0; i<len; i++)
	{
		if(msg[i]==' '){
			if(lastCharWasSpace) return false;
			lastCharWasSpace=true;
		}else{
			lastCharWasSpace=false;
		}
	}
	return true;
}
