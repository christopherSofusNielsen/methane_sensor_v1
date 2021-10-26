/*
 * ADC_module.c
 *
 * Created: 18-10-2021 17:20:16
 *  Author: Mainframe
 */ 

#include <xc.h>

#include "ADC_module.h"
#include "../../HAL/TWI/TWI_API.h"
#include "../../HAL/TWI/TWI_HAL.h"

#define ADC_BIT_RESOLUTION 4096

float Vc=5.00;
float RRL=2500.00;
float PPM_factor=1.00;


ADC_STATUS ADC_init(uint8_t interval){
	uint8_t status;
	
	//Setup timer
	
	//
	status=TWI_HAL_start(ADC_SLAVE_ADDR, WRITE);
	TWI_HAL_stop();
	
	if(status!=TWI_CODE_SUCCESS) return ADC_STATUS_FATAL_ERROR;
	return ADC_STATUS_SUCCESS;
}

ADC_STATUS ADC_get_reading(uint16_t *value){
	uint8_t status;
	uint8_t data[2];
	uint16_t bit_value;
	
	*value=0xffff;
	
	status=TWI_API_read_data_ack_end_nack_stop(ADC_SLAVE_ADDR, data, 2);
	if(status != TWI_CODE_SUCCESS) return ADC_STATUS_ERROR;
	bit_value = (data[0]<<8)|(data[1] & 0xFF);
	*value=calc_ppm_methane(bit_value);
}


uint16_t calc_ppm_methane(uint16_t bit_value){
	float VRL=((float)bit_value/(uint16_t)ADC_BIT_RESOLUTION)*(float)Vc;
	float Rs=(((float)Vc-(float)VRL)/(float)VRL)*(float)RRL;
	uint16_t PPM=(float)Rs*(float)PPM_factor;
	return PPM;
}