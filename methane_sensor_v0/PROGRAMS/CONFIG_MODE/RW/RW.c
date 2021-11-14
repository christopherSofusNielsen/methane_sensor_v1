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
#include "../../../MODULES/RTC_module/RTC_module.h"
#include "../../../HAL/TWI/TWI_HAL.h"

static char arg[80];

static void read_cols(char res[] );
static void read_time(char res[]);
static bool write_deveui(const char cmd[], char res[]);
static bool write_appeui(const char cmd[], char res[]);
static bool write_appkey(const char cmd[], char res[]);
static bool write_PPM_factor(const char cmd[], char res[]);
static bool write_RRL(const char cmd[], char res[]);
static bool write_VCC(const char cmd[], char res[]);
static bool write_cols(const char cmd[], char res[]);
static bool write_heat_up(const char cmd[], char res[]);
static bool write_air_pump(const char cmd[], char res[]);
static bool write_sp_interval(const char cmd[], char res[]);
static bool write_reset(const char cmd[], char res[]);
static bool write_time(const char cmd[], char res[]);
static uint8_t get_time_par(char ts[], uint8_t index);

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
	else if(strcmp(par, RW_HEAT_UP_T)==0)
	{
		uint8_t val=EM_get_heat_up_time();
		sprintf(res, "heat up time=%u m", val);
	}
	else if(strcmp(par, RW_AIR_PUMP_T)==0)
	{
		uint16_t val=EM_get_air_pump_time();
		sprintf(res, "air pump time=%u s", val);
	}
	else if(strcmp(par, RW_SP_INTERVAL)==0)
	{
		uint8_t val=EM_get_sp_interval();
		sprintf(res, "sampling process interval=%u h", val);
	}
	else if(strcmp(par, RW_TIME)==0)
	{
		read_time(res);
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
	else if(strcmp(par, RW_HEAT_UP_T)==0)
	{
		return write_heat_up(cmd, res);
	}
	else if(strcmp(par, RW_AIR_PUMP_T)==0)
	{
		return write_air_pump(cmd, res);
	}
	else if(strcmp(par, RW_SP_INTERVAL)==0)
	{
		return write_sp_interval(cmd, res);
	}
	else if(strcmp(par, RW_RESET)==0)
	{
		return write_reset(cmd, res);
	}
	else if(strcmp(par, RW_TIME)==0)
	{
		return write_time(cmd, res);
	}
	else{
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

static void read_time(char res[]){
	TWI_HAL_init();
	Datetime dt;
	
	RTC_STATUS status=RTC_get_current_time(&dt);
	if(status!=RTC_STATUS_SUCCESS){
		strcpy(res, "Failed to read time");
	}else{
		sprintf(res, "%02u-%02u-%02uT%02u:%02u:%02u", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);	
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

static bool write_heat_up(const char cmd[], char res[]){
	if(!get_parameter(cmd, arg, 2)) return false;
	
	uint8_t val=atoi(arg);
	
	if(val<1 || val>200) return false;
	
	EM_set_heat_up_time(val);
	
	strcpy(res, RW_S_SUCCESS);
	return true;
}

static bool write_air_pump(const char cmd[], char res[]){
	if(!get_parameter(cmd, arg, 2)) return false;
	
	uint16_t val=atoi(arg);
	
	if(val<10 || val>1000) return false;
	
	EM_set_air_pump_time(val);
	
	strcpy(res, RW_S_SUCCESS);
	return true;
}

static bool write_sp_interval(const char cmd[], char res[]){
	if(!get_parameter(cmd, arg, 2)) return false;
	
	uint8_t val=atoi(arg);
	
	if(val<6 || val>24) return false;
	
	EM_set_sp_interval(val);
	
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
	EM_set_heat_up_time(2);
	EM_set_air_pump_time(10);
	EM_set_sp_interval(6);
	
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

static bool write_time(const char cmd[], char res[]){
	TWI_HAL_init();
	if(!get_parameter(cmd, arg, 2)) return false;
	if(strlen(arg)!=17){
		strcpy(res, "Must be in format: yy-mm-ddThh:mm:ss");
		return true;
	}
	
	Datetime dt;
	
	dt.year=get_time_par(arg, 0);
	dt.month=get_time_par(arg, 3);
	dt.day=get_time_par(arg, 6);
	dt.hour=get_time_par(arg, 9);
	dt.minute=get_time_par(arg, 12);
	dt.second=get_time_par(arg, 15);
	
	RTC_STATUS status=RTC_set_current_time(dt);
	if(status!=RTC_STATUS_SUCCESS){
		strcpy(res, "Failed to set time");
		return true;
	}
	strcpy(res, RW_S_SUCCESS);
	return true;
}

static uint8_t get_time_par(char ts[], uint8_t index){
	char par[3];
	par[0]=ts[index];
	par[1]=ts[index+1];
	par[2]='\0';
	
	return atoi(par);
}

