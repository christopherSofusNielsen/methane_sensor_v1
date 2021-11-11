/*
 * ADC_module.c
 *
 * Created: 18-10-2021 17:20:16
 *  Author: Mainframe
 */ 

#include <xc.h>
#include <stdbool.h>
#include <util/delay.h>
#include <stddef.h>
#include "ADC_module.h"
#include "../../HAL/TWI/TWI_API.h"
#include "../../HAL/TWI/TWI_HAL.h"
#include "../../HAL/PM/PM_HAL.h"
#include "../../HAL/TC0/TC0_HAL.h"


#define ADC_BIT_RESOLUTION 4096

static float Vc=5.00;
static float RRL=2500.00;
static float PPM_factor=1.00;
static uint16_t _nSamples;
static uint16_t *_data;
static uint16_t cntSamples;
static uint8_t _powerUpTime;
static uint8_t cntPowerUpTime;

static void vect_ADC_do_sample();
static void vect_ADC_heat_timeout();

ADC_STATUS ADC_init_sampling(uint8_t samplingInterval, uint16_t nSamples, uint16_t data[]){
	_data=data;
	_nSamples=nSamples;
	cntSamples=0;
	
	//Initialize timer
	TC0_HAL_init(samplingInterval, &vect_ADC_do_sample);
	
	//Test that the ADC is active on the bus
	PM_HAL_adc_power(true);
	_delay_ms(400); 
	
	
	uint8_t status=TWI_HAL_start(ADC_SLAVE_ADDR, WRITE);
	TWI_HAL_stop();
	
	if(status!=TWI_CODE_SUCCESS){
		_data=NULL;
		_nSamples=0;
		return ADC_STATUS_FATAL_ERROR;
	}
	return ADC_STATUS_SUCCESS;
}

void ADC_deinit_sampling(){
	TC0_HAL_stop();
	PM_HAL_adc_power(false);
	_data=NULL;
}

ADC_STATUS ADC_start_sampling(){
	TC0_HAL_start();
	vect_ADC_do_sample();
	return ADC_STATUS_SUCCESS;
}

bool ADC_is_sampling_done(){
	return cntSamples>=_nSamples;
}



ADC_STATUS ADC_get_value(uint16_t *value){
	uint8_t status;
	uint8_t data[2];
	uint16_t bit_value;
	
	*value=0xffff;
	
	status=TWI_API_read_data_ack_end_nack_stop(ADC_SLAVE_ADDR, data, 2);
	if(status != TWI_CODE_SUCCESS) return ADC_STATUS_ERROR;
	bit_value = (data[0]<<8)|(data[1] & 0xFF);
	*value=calc_ppm_methane(bit_value);
	return ADC_STATUS_SUCCESS;
}

ADC_STATUS ADC_meth_sens_power_on(uint8_t powerUpTime){
	_powerUpTime=powerUpTime;
	cntPowerUpTime=0;
	
	//Turn on heater
	PM_HAL_meth_power(true);
	
	//Setup timer
	//TC0_HAL_init(60, &vect_ADC_heat_timeout);
	TC0_HAL_init(1, &vect_ADC_heat_timeout);
	TC0_HAL_start();
	return ADC_STATUS_SUCCESS;
}

ADC_STATUS ADC_meth_sens_power_off(){
	PM_HAL_meth_power(false);
	return ADC_STATUS_SUCCESS;
}

bool ADC_meth_sens_ready(){
	return cntPowerUpTime>=_powerUpTime;
}

ADC_STATUS ADC_set_conf_parameters(float Vcc, float Rrl, float PPMfactor){
	Vc=Vcc;
	RRL=Rrl;
	PPM_factor=PPMfactor;
	return ADC_STATUS_SUCCESS;
}

/************************************************************************/
/* Local functions                                                      */
/************************************************************************/
uint16_t calc_ppm_methane(uint16_t bit_value){
	float VRL=((float)bit_value/(uint16_t)ADC_BIT_RESOLUTION)*(float)Vc;
	float Rs=(((float)Vc-(float)VRL)/(float)VRL)*(float)RRL;
	uint16_t PPM=(float)Rs*(float)PPM_factor;
	return PPM;
}

static void vect_ADC_do_sample(){
	ADC_get_value(&(_data[cntSamples++]));
	if(cntSamples>=_nSamples){
		TC0_HAL_stop();
	}
}

static void vect_ADC_heat_timeout(){
	cntPowerUpTime++;
	if(cntPowerUpTime>=_powerUpTime){
		TC0_HAL_stop();
	}
}