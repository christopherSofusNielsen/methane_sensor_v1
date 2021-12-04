/*
 * RN2483_protocol.c
 *
 * Created: 04-10-2021 17:12:47
 *  Author: Christopher S. Nielsen
 */ 

#include "RN2483_protocol.h"
#include <stdio.h>
#include <string.h>

#include <xc.h>

static void tohex(char str[], const uint8_t data[], uint8_t bytes);

/*************************************************************
 * Auxiliary - local
**************************************************************/

static void tohex(char str[], const uint8_t data[], uint8_t bytes)
{
    for (uint8_t i = 0; i < bytes; i++){
        sprintf(str + i * 2, "%02x", data[i]);
	}
}

/*************************************************************
 * Auxiliary - global
**************************************************************/

void attach_ending(char buffer[])
{
    strcat(buffer, CMD_END_CHARS);
}

int16_t cmd_with_ending(char buffer[], const char cmd[]){
	strcpy(buffer, cmd);
	strcat(buffer, CMD_END_CHARS);
	return strlen(buffer);
}

/*************************************************************
 * Sys commands
**************************************************************/

int16_t sys_sleep_cmd(char buffer[], uint32_t time)
{
    if (time < 100 || time >= 4294967296)
        return -1;

    sprintf(buffer, "%s %ld", SYS_SLEEP, time);
    return strlen(buffer);
}

/*************************************************************
 * Mac commands
**************************************************************/

int16_t mac_set_parameter(char buffer[], const char *cmd, const char *parameter)
{
    sprintf(buffer, "%s %s", cmd, parameter);
    return strlen(buffer);
}


int16_t mac_set_deveui(char buffer[], char *eui)
{
    if (strlen(eui) != 16)
        return -1;
    return mac_set_parameter(buffer, SET_DEVEUI, eui);
}

int16_t mac_set_appeui(char buffer[], char *appeui)
{
    if (strlen(appeui) != 16)
        return -1;
    return mac_set_parameter(buffer, SET_APPEUI, appeui);
}

int16_t mac_set_appkey(char buffer[], char *appkey)
{
    if (strlen(appkey) != 32)
        return -1;
    return mac_set_parameter(buffer, SET_APPKEY, appkey);
}

int16_t mac_set_ADR(char buffer[], unsigned char state){
	if(state==0){
		return mac_set_parameter(buffer, SET_ADR, OFF);
	}else{
		return mac_set_parameter(buffer, SET_ADR, ON);
	}
}

int16_t mac_tx_confirmed(char buffer[], uint8_t port,  uint8_t data[], uint8_t bytes)
{
	char dataHex[MAX_PAYLOAD_SIZE_EU868 * 2 + 1];
	
    if (port < PORT_MIN || port > PORT_MAX)
        return -1;

    if (bytes > MAX_PAYLOAD_SIZE_EU868 || bytes <= 0)
        return -1;
	
    
    tohex(dataHex, data, bytes);

    sprintf(buffer, "%s %u %s", MAC_TX_CONFIRMED, port, (const char *)dataHex);
    
	
    return strlen(buffer);
}
