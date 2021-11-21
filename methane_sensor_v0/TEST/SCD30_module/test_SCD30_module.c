/*
 * test_SCD30_module.c
 *
 * Created: 14-10-2021 08:42:54
 *  Author: Mainframe
 */ 

#include <xc.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "test_SCD30_module.h"
#include "../../HAL/TWI/TWI_HAL.h"
#include "../../HAL/TWI/TWI_API.h"
#include "../../HAL/UART1/UART1_HAL.h"
#include "../../MODULES/SCD30_module/SCD30_module.h"
#include "../../HAL/PM/PM_HAL.h"
#include "../../util/bit_operators.h"


static void test_sampling();
static void test_get_reading();
static void power_test();

//void read_firmware_api();
//void throw_error(uint8_t status, uint8_t index);
//void read_measure_interval();

void test_SCD30_module_start(){
		uart1_hal_init();
		PM_HAL_SCD30_power_init();
		TWI_HAL_init();
		
			
		while(1){
			//test_sampling();
			//test_get_reading();
			power_test();
			
			_delay_ms(1500);
		}
}

/************************************************************************/
/* Test init of SCD30 and timer                                         */
/************************************************************************/
static void test_sampling(){
	uint16_t data[5];
	uart1_hal_send_string("RUN ");
	SCD30_STATUS status=SCD30_sensor_on();
	SCD30_init_sampling(3, 5, data);
	if(status!=SCD30_STATUS_SUCCESS){
		uart1_hal_send_string("FAIL ");
		return;
	}
	
	SCD30_start_sampling();
	while(!SCD30_is_sampling_done()){};
	
	SCD30_deinit_sampling();
	
	for (uint8_t i=0; i<5; i++)
	{
		char msg[20];
		sprintf(msg, " %u ", data[i]);
		uart1_hal_send_string(msg);
	}
}

static void test_get_reading(){
	SCD30_STATUS status;
	uint16_t value;
	
	status=SCD30_sensor_on();
	if(status!=SCD30_STATUS_SUCCESS){
		uart1_hal_send_string("Failed ");
		return;
	}
	
	status=SCD30_get_reading(&value);
	if(status!=SCD30_STATUS_SUCCESS){
		uart1_hal_send_string("Failed ");
		return;
	}
	
	char msg[20];
	sprintf(msg, " %u ", value);
	uart1_hal_send_string(msg);
}

static void power_test(){
	uint16_t data[5];
	uart1_hal_send_string("Start SCD30 power test");
	
	//Turn on
	PM_HAL_SCD30_power(true);
	
	//Sample
	SCD30_STATUS status=SCD30_sensor_on();
	if(status!=SCD30_STATUS_SUCCESS){
		uart1_hal_send_string("FAIL ");
		return;
	}
	
	SCD30_init_sampling(2, 5, data);
	SCD30_start_sampling();
	
	while(!SCD30_is_sampling_done()){};
	
	uart1_hal_send_string("Done, power off");
	SCD30_deinit_sampling();
	PM_HAL_SCD30_power(false);
	_delay_ms(2000);
	uart1_hal_send_string("End SCD30 power test");
}



/************************************************************************/
/* Other test functions                                                 */
/************************************************************************/

//void read_firmware_api(){
	//uint8_t status;
	//uint8_t write[]={0xD1, 0x00};
	//uint8_t read[3];
	//TWI_HAL_init();
	//status=TWI_API_write_data_stop(0x61, write, 2);
	//if(status!=TWI_CODE_SUCCESS)
		//return throw_error(status, 0);
	//_delay_ms(3);
	//
	//status=TWI_API_read_data_ack_end_nack_stop(0x61, read, 3);
	//if(status!=TWI_CODE_SUCCESS)
		//return throw_error(status, 1);
	//
	//uart1_hal_send_message(read, 3);
//}
//
//void throw_error(uint8_t status, uint8_t index){
	//uint8_t msg[3];
	//msg[0]=status;
	//msg[1]=index;
	//msg[2]=0xFF;
	//uart1_hal_send_message(msg, 3);
//}

