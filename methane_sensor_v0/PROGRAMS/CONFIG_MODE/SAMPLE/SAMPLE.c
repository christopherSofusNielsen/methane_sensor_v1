/*
 * SAMPLE.c
 *
 * Created: 11-11-2021 14:55:35
 *  Author: Mainframe
 */ 
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>
#include <stdio.h>

#include "SAMPLE.h"
#include "../UTIL/util.h"
#include "../../../HAL/PM/PM_HAL.h"
#include "../../../HAL/TWI/TWI_HAL.h"
#include "../../../MODULES/ADC_module/ADC_module.h"
#include "../../../MODULES/SCD30_module/SCD30_module.h"
#include "../../../MODULES/EEPROM_module/EEPROM_module.h"

static char par[20];


static bool handle_methane(const char cmd[], char res[]);
static bool handle_co2(const char cmd[], char res[]);
static bool handle_pump(const char cmd[], char res[]);

bool handle_sample(const char cmd[], char res[]){
	if(!get_parameter(cmd, par, 1)) return false;
	
	if(strcmp(par, S_METH)==0){
		return handle_methane(cmd, res);
	}else if(strcmp(par, S_CO2)==0){
		return handle_co2(cmd, res);
	}else if(strcmp(par, S_PUMP)==0){
		return handle_pump(cmd, res);
	}else{
		strcpy(res, CONF_STAT_NOT_EXIST);
		return true;
	}
}

bool init_methane_SCD30(){
	TWI_HAL_init();
	PM_HAL_adc_power_init();
	PM_HAL_adc_power(true);
	PM_HAL_SCD30_power_init();
	PM_HAL_SCD30_power(true);
	SCD30_STATUS status=SCD30_sensor_on();
	if(status!= SCD30_STATUS_SUCCESS) return false;
	return true;
	_delay_ms(500);
}


static bool handle_methane(const char cmd[], char res[]){
	if(!get_parameter(cmd, par, 2)) return false;
	
	if(strcmp(par, METH_POWER_ON)==0){
		PM_HAL_meth_power_init();
		PM_HAL_meth_power(true);
		
		strcpy(res, "Power methane sensor on");
		return true;
	}else if(strcmp(par, METH_POWER_OFF)==0){
		PM_HAL_meth_power_init();
		PM_HAL_meth_power(false);
		
		strcpy(res, "Power methane sensor off");
		return true;
	}
	else if(strcmp(par, METH_READ)==0)
	{
		//Set parameters
		float vccx=0.0;
		float rrlx=0.0;
		float ppmx=0.0;
		
		EM_get_Vcc(&vccx);
		EM_get_RRL(&rrlx);
		EM_get_ppmfactor(&ppmx);
		
		ADC_set_conf_parameters(vccx, rrlx, ppmx);
		
		
		uint16_t val;
		
		if (ADC_get_value(&val)!=ADC_STATUS_SUCCESS)
		{
			strcpy(res, "Error when reading data");
		}else{
			sprintf(res, "PPM=%u", val);
		}
		return true;
	}else{
		strcpy(res, CONF_STAT_NOT_EXIST);
		return true;
	}
	return false;
}

static bool handle_co2(const char cmd[], char res[]){
	if(!get_parameter(cmd, par, 2)) return false;
	
	if(strcmp(par, CO2_READ)==0){
		uint16_t val;
		if(SCD30_get_reading(&val)!=SCD30_STATUS_SUCCESS){
			strcpy(res, "Error when reading data");
		}else{
			sprintf(res, "PPM=%u", val);
		}
		return true;
	}else{
		strcpy(res, CONF_STAT_NOT_EXIST);
		return true;
	}
}

static bool handle_pump(const char cmd[], char res[]){
	if(!get_parameter(cmd, par, 2)) return false;
	
	if(strcmp(par, PUMP_ON)==0){
		PM_HAL_BC_power_init();
		PM_HAL_BC_power(true);
		
		strcpy(res, "Power to air pump on");
		return true;
	}else if(strcmp(par, PUMP_OFF)==0){
		PM_HAL_BC_power_init();
		PM_HAL_BC_power(false);
		
		strcpy(res, "Power to air pump off");
		return true;
	}else{
		strcpy(res, CONF_STAT_NOT_EXIST);
		return true;
	}
}



