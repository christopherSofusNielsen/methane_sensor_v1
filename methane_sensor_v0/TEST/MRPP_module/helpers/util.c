/*
 * util.c
 *
 * Created: 05-11-2021 14:27:39
 *  Author: Mainframe
 */ 
#include "util.h"
#include <stdint.h>
#include <stdio.h>

void util_fill_data(uint16_t arr[], uint16_t data, uint16_t length){
	for (uint16_t i = 0; i < length; i++)
	{
		arr[i]=data;
	}
}


void util_2_hex_str(char str[], uint8_t data[], uint8_t length){
	for (uint8_t i = 0; i < length; i++)
	{
		sprintf(str, "%02X ", data[i]);
		str+=3;
	}
}
