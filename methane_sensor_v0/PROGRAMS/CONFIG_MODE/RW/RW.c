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

static char arg[80];

static bool write_deveui(const char cmd[], char res[]);
static bool write_PPM_factor(const char cmd[], char res[]);

bool handle_read(const char cmd[], char res[]){
	char par[20];
	if(!get_parameter(cmd, par, 1)) return false;
	
	if (strcmp(par, "deveui")==0)
	{
		strcpy(res, "DEVEUI");
	}else if (strcmp(par, "hweui")==0)
	{
		strcpy(res, "HWEUI");
	}else if (strcmp(par, "appeui")==0){
		strcpy(res, "APPEUI");
	}
	
	return true;
}

bool handle_write(const char cmd[], char res[]){
	char par[20];
	if(!get_parameter(cmd, par, 1)) return false;
	
	if (strcmp(par, "deveui")==0)
	{
		return write_deveui(cmd, res);
	}else if(strcmp(par, "ppmx")==0){
		return write_PPM_factor(cmd, res);
	}
	return true;
}

static bool write_deveui(const char cmd[], char res[]){
	if(!get_parameter(cmd, arg, 2)) return false;
	
	strcpy(res, arg);
	
	return true;
}

static bool write_PPM_factor(const char cmd[], char res[]){
	if(!get_parameter(cmd, arg, 2)) return false;
	
	float val=str2flaot(arg);

	
	sprintf(res, "%.2f", val);
	
	
	return true;
}






