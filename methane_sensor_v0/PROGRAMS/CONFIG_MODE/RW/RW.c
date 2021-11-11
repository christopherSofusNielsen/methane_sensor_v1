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


#include "RW.h"
#include "../UTIL/util.h"
#include "../../../util/str2float/str2float.h"
#include "../../../MODULES/EEPROM_module/EEPROM_module.h"

static char arg[80];

static bool write_deveui(const char cmd[], char res[]);
static bool write_appeui(const char cmd[], char res[]);
static bool write_appkey(const char cmd[], char res[]);
static bool write_PPM_factor(const char cmd[], char res[]);
static bool write_RRL(const char cmd[], char res[]);
static bool write_VCC(const char cmd[], char res[]);

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
	return true;
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



