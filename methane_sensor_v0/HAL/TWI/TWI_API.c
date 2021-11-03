/*
 * TWI_API.c
 *
 * Created: 15-10-2021 15:27:52
 *  Author: Mainframe
 */ 

#include "TWI_API.h"
#include "TWI_HAL.h"

//uint8_t __locked;
//#define _lock __locked=0x01
//#define _free __locked=0x00
//#define _is_free __locked==0x00


static uint8_t start_and_write_data(uint8_t slaveAddr, const uint8_t data[], uint8_t length);
static uint8_t read_and_stop(uint8_t data[], uint8_t length);

uint8_t TWI_API_write_data(uint8_t slaveAddr, const uint8_t data[], uint8_t length){
	return start_and_write_data(slaveAddr, data, length);
}

uint8_t TWI_API_write_data_stop(uint8_t slaveAddr, const uint8_t data[], uint8_t length){
	uint8_t status=start_and_write_data(slaveAddr, data, length);
	if(status!=TWI_CODE_SUCCESS) return status;
	TWI_HAL_stop();
	return TWI_CODE_SUCCESS;
}

uint8_t TWI_API_read_data_ack_end_nack_stop(uint8_t slaveAddr, uint8_t data[], uint8_t length){
	uint8_t status;
	
	status=TWI_HAL_start(slaveAddr, READ);
	if(status!=TWI_CODE_SUCCESS) return status;
	
	return read_and_stop(data, length);
	
	//for (uint8_t i=0; i<length-1; i++)
	//{
		//data[i]=TWI_HAL_read_byte_ack();
	//}
	//data[length-1]=TWI_HAL_read_byte_nack();
	//
	//TWI_HAL_stop();
	//return TWI_CODE_SUCCESS;
}

uint8_t TWI_API_repeat_read_data_stop(uint8_t slaveAddr, uint8_t data[], uint8_t length){
	uint8_t status;
	
	status=TWI_HAL_repeated_start(slaveAddr);
	if(status!=TWI_CODE_SUCCESS) return status;
	
	return read_and_stop(data, length);
	
}




/************************************************************************/
/* Local function                                                                     */
/************************************************************************/

static uint8_t start_and_write_data(uint8_t slaveAddr, const uint8_t data[], uint8_t length){
	uint8_t status;
	
	status=TWI_HAL_start(slaveAddr, WRITE);
	if(status!=TWI_CODE_SUCCESS) return status;
	
	for (uint8_t i=0; i<length; i++)
	{
		status=TWI_HAL_write_byte(data[i]);
		if(status!=TWI_CODE_SUCCESS) return status;
	}
	return TWI_CODE_SUCCESS;
}

static uint8_t read_and_stop(uint8_t data[], uint8_t length){
	for (uint8_t i=0; i<length-1; i++)
	{
		data[i]=TWI_HAL_read_byte_ack();
	}
	data[length-1]=TWI_HAL_read_byte_nack();
	
	TWI_HAL_stop();
	return TWI_CODE_SUCCESS;
}

	