/*
 * util.c
 *
 * Created: 09-11-2021 18:10:12
 *  Author: Mainframe
 */ 
#include "util.h"

#include <xc.h>
#include <stdbool.h>
#include <string.h>

bool get_parameter(const char src[], char par[], uint8_t parNo){
	uint8_t pars=count_parameters(src);
	if(pars<parNo) return false;
	
	uint16_t len=strlen(src);
	uint16_t startIndex=par_index(src, parNo);
	uint8_t parLen=0;
	
	for (uint8_t i=startIndex; i<len; i++)
	{
		if(src[i]==' ') break;
		par[parLen++]=src[i];
	}
	par[parLen]='\0';
	
	return true;
}

uint8_t count_parameters(const char src[]){
	uint16_t len=strlen(src);
	uint8_t cnt=0;
	
	for (uint16_t i=0; i<len; i++)
	{
		if(src[i]==' '){
			cnt++;
		}
	}
	return cnt;
}

uint16_t par_index(const char src[], uint8_t parNo){
	uint8_t cnt=0;
	
	if(parNo==0) return 0;
	
	for (uint16_t i=0; i<strlen(src); i++)
	{
		if(src[i]==' '){
			cnt++;
			if(cnt==parNo){
				return i+1;
			}
		}
	}
	return 0;
}