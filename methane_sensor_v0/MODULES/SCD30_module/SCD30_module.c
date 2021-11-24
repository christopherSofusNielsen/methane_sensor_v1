/*
 * SCD30_module.c
 *
 * Created: 17-10-2021 14:53:00
 *  Author: Christopher S. Nielsen
 */ 

#include <util/delay.h>
#include <stdbool.h>
#include <stddef.h>
#include "SCD30_module.h"
#include "../../HAL/TWI/TWI_API.h"
#include "../../HAL/SCD30/SCD30_HAL.h"
#include "../../util/CRC8/CRC8.h"
#include "../../util/to_float.h"
#include "../../HAL/PM/PM_HAL.h"
#include "../../HAL/TC1/TC1_HAL.h"

/************************************************************************/
/* Constants                                                            */
/************************************************************************/
const uint8_t TRIGGER_CONT_MEASUREMENT[]={0x00, 0x10, 0x00, 0x00, 0x81};
const uint8_t SET_MEASUREMENT_INTERVAL[]={0x46, 0x00, 0x00, 0x02, 0xE3};
const uint8_t READ_DATA_CMD[]={0x03, 0x00};
const uint8_t DATA_READY_CMD[]={0x02, 0x02};

/************************************************************************/
/* Variables                                                            */
/************************************************************************/
static uint16_t _nSamples;
static uint16_t *_data;
static uint16_t cntSamples;
	
/************************************************************************/
/* Functions                                                            */
/************************************************************************/
static bool validate_data(uint8_t data[]);
static uint16_t bytes_2_uint(uint8_t data[]);
static void vect_SCD_do_sample();
static SCD30_STATUS read_value(uint16_t *value);
static SCD30_STATUS init_measurement();

SCD30_STATUS SCD30_sensor_on(){
	SCD30_HAL_init();
	PM_HAL_SCD30_power(true);
	_delay_ms(2000);
	return init_measurement();
}

void SCD30_sensor_off(){
	PM_HAL_SCD30_power(false);
}

void SCD30_init_sampling(uint16_t samplingInterval, uint16_t nSamples, uint16_t data[]){
	_nSamples=nSamples;
	cntSamples=0;
	_data=data;

	//Init
	TC1_HAL_init(samplingInterval, &vect_SCD_do_sample);
}

void SCD30_deinit_sampling(){
	TC1_HAL_stop();
	_data=NULL;
}

void SCD30_start_sampling(){
	vect_SCD_do_sample();
	TC1_HAL_start();
}

bool SCD30_is_sampling_done(){
	return cntSamples>=_nSamples;
}

SCD30_STATUS SCD30_get_reading(uint16_t *value){
	while(!SCD30_HAL_data_ready()){};
	
	return read_value(value);
}


/************************************************************************/
/* Local functions                                                      */
/************************************************************************/

static bool validate_data(uint8_t data[]){
	uint8_t crc=calc_crc8(data, 2);
	if(data[2]!=crc) return false;
	
	crc=calc_crc8(&data[3], 2);
	if(data[5]!=crc) return false;
	
	return true;
}

static uint16_t bytes_2_uint(uint8_t data[]){
	
	to_float f;
	
	f.bytes[3]=data[0];
	f.bytes[2]=data[1];
	f.bytes[1]=data[3];
	f.bytes[0]=data[4];
	
	return (uint16_t) f.f;
}

static SCD30_STATUS init_measurement(){
	uint8_t status;
	status=TWI_API_write_data_stop(SCD30_ADDR,  SET_MEASUREMENT_INTERVAL, 5);
	if(status != TWI_CODE_SUCCESS) return SCD30_STATUS_FATAL_ERROR;
	
	status=TWI_API_write_data_stop(SCD30_ADDR,  TRIGGER_CONT_MEASUREMENT, 5);
	if(status != TWI_CODE_SUCCESS) return SCD30_STATUS_FATAL_ERROR;
	
	//Wait for data_ready pin to go high
	while(!SCD30_HAL_data_ready()){};
	
	return SCD30_STATUS_SUCCESS;
}

static SCD30_STATUS read_value(uint16_t *value){
	uint8_t status;
	uint8_t data[6];
	*value=0xffff;

	if(!SCD30_HAL_data_ready()){
		*value=0xfffe;
		return SCD30_STATUS_TRY_AGAIN;
	}
	
	
	status=TWI_API_write_data_stop(SCD30_ADDR,   READ_DATA_CMD, 2);
	if(status != TWI_CODE_SUCCESS) return SCD30_STATUS_ERROR;
	
	_delay_ms(4);
	
	status=TWI_API_read_data_ack_end_nack_stop(SCD30_ADDR, data, 6);
	if(status != TWI_CODE_SUCCESS) return SCD30_STATUS_ERROR;
	
	if(!validate_data(data)) return SCD30_STATUS_ERROR;
	
	*value=bytes_2_uint(data);
	
	return SCD30_STATUS_SUCCESS;
}

/************************************************************************/
/* Interrupts                                                           */
/************************************************************************/
static void vect_SCD_do_sample(){
	read_value(&(_data[cntSamples++]));
	if(cntSamples>=_nSamples){
		TC1_HAL_stop();
	}
}

