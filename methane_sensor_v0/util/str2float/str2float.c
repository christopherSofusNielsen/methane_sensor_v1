/*
 * str2float.c
 *
 * Created: 09-11-2021 19:41:16
 *  Author: Christopher S. Nielsen
 */ 

#include "str2float.h"
#include <stdlib.h>
#include <string.h>

static float get_decimal(char str[]);

float str2flaot(char str[]){
	
	uint16_t intVal=atoi(str);
	float deci=get_decimal(str);
	return intVal+deci;
	
	
}

static float get_decimal(char str[]){
	uint8_t index=0;
	
	for (uint8_t i=0; i<strlen(str)-1; i++)
	{
		if(str[i]=='.'){
			index=i+1;
			break;
		}
	}
	
	if(index==0) return 0.0000;
	uint8_t len=strlen(&str[index]);
	uint16_t val=atoi(&str[index]);
	float divisor=1;
	
	while(len>0){
		divisor*=10;
		len--;
	}
	
	return ((float) val)/divisor;
}

bool is_float(char str[]){
	bool got_dot=false;
	for (uint8_t i=0; i<strlen(str); i++)
	{
		if(str[i]=='.'){
			if (got_dot)
			{
				return false;
			}else{
				got_dot=true;
				continue;
			}
		}
		
		//Check for digitis
		if(str[i]<'0' || str[i]>'9') return false;
	}
	return true;
}