/*
 * test_SCD30_module.c
 *
 * Created: 14-10-2021 08:42:54
 *  Author: Mainframe
 */ 


#define F_CPU 8000000UL	

#include <xc.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>



#include "test_SCD30_module.h"
#include "../../HAL/TWI/TWI_HAL.h"
#include "../../HAL/UART1/UART1_HAL.h"
#include "../../HAL/TWI/TWI_API.h"
#include "../../MODULES/SCD30_module/SCD30_module.h"
#include "../../HAL/PM/PM_HAL.h"




void read_firmware_api();
void throw_error(uint8_t status, uint8_t index);
static void test_sampling();
void test_get_reading();
void read_measure_interval();

void test_SCD30_module_start(){
		uart1_hal_init();
		PM_HAL_SCD30_power_init();
		
		while(1){
			//read_firmware_api();
			test_sampling();
			//test_get_reading();
			

		
			_delay_ms(1500);
		}
	
	
}

/************************************************************************/
/* Test init of SCD30 and timer                                         */
/************************************************************************/
static void test_sampling(){
	uint16_t data[10];
	
	SCD30_STATUS status=SCD30_init_sampling(20000, 10, data);
	if(status!=SCD30_STATUS_SUCCESS){
		uart1_hal_send_string("FAIL ");
	}
	
	
	SCD30_start_sampling();
	while(!SCD30_is_sampling_done()){};
	
	for (uint8_t i=0; i<10; i++)
	{
		char msg[20];
		sprintf(msg, " %u ", data[i]);
		uart1_hal_send_string(msg);
		_delay_ms(200);
	}
}

void test_get_reading(){
	
	uint16_t value;
	SCD30_get_reading(&value);
	
	char msg[20];
	sprintf(msg, " %u ", value);
	
	uart1_hal_send_string(msg);
}




void read_firmware_api(){
	uint8_t status;
	uint8_t write[]={0xD1, 0x00};
	uint8_t read[3];
	TWI_HAL_init();
	status=TWI_API_write_data_stop(0x61, write, 2);
	if(status!=TWI_CODE_SUCCESS)
		return throw_error(status, 0);
	_delay_ms(3);
	
	status=TWI_API_read_data_ack_end_nack_stop(0x61, read, 3);
	if(status!=TWI_CODE_SUCCESS)
		return throw_error(status, 1);
	
	uart1_hal_send_message(read, 3);
}

void throw_error(uint8_t status, uint8_t index){
	uint8_t msg[3];
	msg[0]=status;
	msg[1]=index;
	msg[2]=0xFF;
	uart1_hal_send_message(msg, 3);
}

