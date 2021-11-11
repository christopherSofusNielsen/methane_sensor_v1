/*
 * RW.c
 *
 * Created: 09-11-2021 14:55:49
 *  Author: Mainframe
 */ 
#include <stdbool.h>
#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "RW.h"
#include "../UTIL/util.h"
#include "../../../util/str2float/str2float.h"
#include "../../../MODULES/EEPROM_module/EEPROM_module.h"
#include "../../../MODULES/MRPP_module/mrpp.h"
#include "../../../MODULES/MRPP_module/types.h"

static char arg[80];

static void read_cols(char res[] );
static bool write_deveui(const char cmd[], char res[]);
static bool write_appeui(const char cmd[], char res[]);
static bool write_appkey(const char cmd[], char res[]);
static bool write_PPM_factor(const char cmd[], char res[]);
static bool write_RRL(const char cmd[], char res[]);
static bool write_VCC(const char cmd[], char res[]);
static bool write_cols(const char cmd[], char res[]);
static bool write_reset(const char cmd[], char res[]);

bool handle_read(const char cmd[], char res[]){
	char par[20];
	float val;
	
	if(!get_parameter(cmd, par, 1) || count_parameters(cmd)==2) return false;
	
	if (strcmp(par, RW_DEVEUI)==0)
	{
		EM_get_deveui(res);
	}
	else if (strcmp(par, RW_APPEUI)==0)
	{
		EM_get_appeui(res);
	}
	else if (strcmp(par, RW_APPKEY)==0)
	{
		EM_get_appkey(res);
	}
	else if(strcmp(par, RW_PPM_FACTOR)==0)
	{
		EM_get_ppmfactor(&val);
		sprintf(res, "%.2f", val);
	}
	else if(strcmp(par, RW_RRL)==0)
	{
		EM_get_RRL(&val);
		sprintf(res, "%.2f", val);
	}
	else if(strcmp(par, RW_VCC)==0)
	{
		EM_get_Vcc(&val);
		sprintf(res, "%.2f", val);
	}
	else if(strcmp(par, RW_COLS)==0)
	{
		read_cols(res);
	}
	else{
		strcpy(res, RW_S_NOT_EXIST);
	}
	return true;
}

bool handle_write(const char cmd[], char res[]){
	char par[20];
	if(!get_parameter(cmd, par, 1)) return false;
	
	if (strcmp(par, RW_DEVEUI)==0)
	{
		return write_deveui(cmd, res);
	}
	else if(strcmp(par, RW_APPEUI)==0)
	{
		return write_appeui(cmd, res);
	}
	else if(strcmp(par, RW_APPKEY)==0)
	{
		return write_appkey(cmd, res);
	}
	else if(strcmp(par, RW_PPM_FACTOR)==0)
	{
		return write_PPM_factor(cmd, res);
	}
	else if(strcmp(par, RW_RRL)==0)
	{
		return write_RRL(cmd, res);
	}
	else if(strcmp(par, RW_VCC)==0)
	{
		return write_VCC(cmd, res);
	}
	else if(strcmp(par, RW_COLS)==0)
	{
		return write_cols(cmd, res);
	}
	else if(strcmp(par, RW_RESET)==0)
	{
		return write_reset(cmd, res);
	}else{
		strcpy(res, RW_S_NOT_EXIST);
		return true;
	}
	
}

/************************************************************************/
/* Read functions                                                       */
/************************************************************************/
static void read_cols(char res[]){
	COLLECTION cols[7];
	EM_get_collections(cols, sizeof(COLLECTION)*7);
	res[0]='\0';
	for (uint8_t i=0; i<7; i++)
	{
		sprintf(arg, "ID=%u, n=%u, si=%u \r\n", i+1, cols[i].samplings, cols[i].samplingInterval);
		strcat(res, arg);
	}
}

/************************************************************************/
/* Write functions                                                      */
/************************************************************************/
static bool write_deveui(const char cmd[], char res[]){
	if(!get_parameter(cmd, arg, 2)) return false;
	
	if(strlen(arg)!=16) return false;
	
	if(!EM_set_deveui(arg)){
		strcpy(res, RW_S_FAILED);
		return false;
	}else{
		strcpy(res, RW_S_SUCCESS);
		return true;
	}
}

static bool write_appeui(const char cmd[], char res[]){
	if(!get_parameter(cmd, arg, 2)) return false;
	
	if(strlen(arg)!=16) return false;
	
	if(!EM_set_appeui(arg)){
		strcpy(res, RW_S_FAILED);
		return false;
	}else{
		strcpy(res, RW_S_SUCCESS);
		return true;
	}
}

static bool write_appkey(const char cmd[], char res[]){
	if(!get_parameter(cmd, arg, 2)) return false;
	
	if(strlen(arg)!=32) return false;
	
	if(!EM_set_appkey(arg)){
		strcpy(res, RW_S_FAILED);
		return false;
	}else{
		strcpy(res, RW_S_SUCCESS);
		return true;
	}
}

static bool write_PPM_factor(const char cmd[], char res[]){
	if(!get_parameter(cmd, arg, 2)) return false;
	
	if(!is_float(arg)) return false;
	
	float val=str2flaot(arg);
	EM_set_ppmfactor(val);
	
	strcpy(res, RW_S_SUCCESS);
	return true;
}


static bool write_RRL(const char cmd[], char res[]){
	if(!get_parameter(cmd, arg, 2)) return false;
	
	if(!is_float(arg)) return false;
	
	float val=str2flaot(arg);
	EM_set_RRL(val);
	
	strcpy(res, RW_S_SUCCESS);
	return true;
}

static bool write_VCC(const char cmd[], char res[]){
	if(!get_parameter(cmd, arg, 2)) return false;
	
	if(!is_float(arg)) return false;
	
	float val=str2flaot(arg);
	EM_set_Vcc(val);
	
	strcpy(res, RW_S_SUCCESS);
	return true;
}

static bool write_cols(const char cmd[], char res[]){
	uint8_t index=0;
	uint16_t samplings=0;
	uint16_t samplingInterval=0;
	
	if(!get_parameter(cmd, arg, 2)) return false;
	index=atoi(arg);
	if(index<1 || index>7) return false;
	index--; //Offset index
	
	if(!get_parameter(cmd, arg, 3)) return false;
	samplings=atoi(arg);
	if(samplings<2 || samplings>300) return false;
	
	if(!get_parameter(cmd, arg, 4)) return false;
	samplingInterval=atoi(arg);
	if(samplingInterval<2 || samplingInterval>600) return false;
	
	COLLECTION cols[7];
	EM_get_collections(cols, sizeof(COLLECTION)*7);
	
	cols[index].samplings=samplings;
	cols[index].samplingInterval=samplingInterval;
	
	EM_set_collections(cols, sizeof(COLLECTION)*7);
	
	strcpy(res, RW_S_SUCCESS);
	return true;	
}

static bool write_reset(const char cmd[], char res[]){
	
	COLLECTION cols[7];
	
	EM_set_deveui("0000000000000000");
	EM_set_appeui("0000000000000000");
	EM_set_appkey("00000000000000000000000000000000");
	EM_set_ppmfactor(1.00);
	EM_set_RRL(2500.00);
	EM_set_Vcc(5.00);
	
	for (uint8_t i=0; i<7; i++)
	{
		cols[i].type=T_INT16;
		cols[i].samplings=2;
		cols[i].samplingInterval=10;
	}
	
	EM_set_collections(cols, sizeof(COLLECTION)*7);
	
	strcpy(res, "All settings reset");
	return true;
}

/************************************************************************/
/* 

00000000000000000000000000000000
EA11A4D6D368D5CDE102B0491B40A494
                                                                     */
/************************************************************************/