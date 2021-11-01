/*
 * SCD30_module.c
 *
 * Created: 17-10-2021 14:53:00
 *  Author: Mainframe
 */ 

#include <util/delay.h>
#include <stdbool.h>
#include "SCD30_module.h"
#include "../../HAL/TWI/TWI_API.h"
#include "../../HAL/TWI/TWI_HAL.h"
#include "../../HAL/SCD30/SCD30_HAL.h"
#include "../../util/CRC8/CRC8.h"
#include "../../util/to_float.h"
#include "../../HAL/PM/PM_HAL.h"

/************************************************************************/
/* Constants                                                            */
/************************************************************************/
#define SCD30_ADDR 0x61

const uint8_t TRIGGER_CONT_MEASUREMENT[]={0x00, 0x10, 0x00, 0x00, 0x81};
const uint8_t SET_MEASUREMENT_INTERVAL[]={0x46, 0x00, 0x00, 0x02, 0xE3};
const uint8_t READ_DATA_CMD[]={0x03, 0x00};
const uint8_t DATA_READY_CMD[]={0x02, 0x02};

/************************************************************************/
/* Variables                                                            */
/************************************************************************/
uint16_t _nSamples;
uint16_t *_data;
uint16_t cntSamples;
	
/************************************************************************/
/* Functions                                                            */
/************************************************************************/
static bool validate_data(uint8_t data[]);
static uint16_t bytes_2_uint(uint8_t data[]);



SCD30_STATUS SCD30_init(uint16_t samplingInterval, uint16_t nSamples, uint16_t data[]){
	uint8_t status;
	_nSamples=nSamples;
	cntSamples=0;
	_data=data;
	
	//Init
	SCD30_HAL_init();
	
	PM_HAL_SCD30_power(true);
	
	
	status=TWI_API_write_data_stop(SCD30_ADDR,  SET_MEASUREMENT_INTERVAL, 5);
	if(status != TWI_CODE_SUCCESS) return SCD30_STATUS_FATAL_ERROR;
	
	status=TWI_API_write_data_stop(SCD30_ADDR,  TRIGGER_CONT_MEASUREMENT, 5);
	if(status != TWI_CODE_SUCCESS) return SCD30_STATUS_FATAL_ERROR;
	
	//Wait for data_ready pin to go high
	while(!SCD30_HAL_data_ready()){};
		
	return SCD30_STATUS_SUCCESS;
}

SCD30_STATUS SCD30_get_reading(uint16_t *value){
	uint8_t status;
	uint8_t data[6];
	*value=0xffff;

	if(!SCD30_HAL_data_ready()) return SCD30_STATUS_TRY_AGAIN;	
	
	status=TWI_API_write_data_stop(SCD30_ADDR,   READ_DATA_CMD, 2);
	if(status != TWI_CODE_SUCCESS) return SCD30_STATUS_ERROR;
	
	_delay_ms(4); 
	
	status=TWI_API_read_data_ack_end_nack_stop(SCD30_ADDR, data, 6);
	if(status != TWI_CODE_SUCCESS) return SCD30_STATUS_ERROR;
	
	if(!validate_data(data)) return SCD30_STATUS_ERROR;
	
	*value=bytes_2_uint(data);
	
	return SCD30_STATUS_SUCCESS;
}

bool SCD30_data_ready(){
	uint8_t status;
	uint8_t data[3];
	
	status=TWI_API_write_data_stop(SCD30_ADDR,  DATA_READY_CMD, 2);
	if(status != TWI_CODE_SUCCESS) return false;
	
	_delay_ms(4); 
	
	status=TWI_API_read_data_ack_end_nack_stop(SCD30_ADDR, data, 3);
	if(status != TWI_CODE_SUCCESS) return false;
	
	if(data[1]==0x01) return true;
	return false;
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


