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



#include "test_SCD30_module.h"
#include "../../HAL/TWI/TWI_HAL.h"
#include "../../HAL/UART1/UART1_HAL.h"
#include "../../HAL/TWI/TWI_API.h"
#include "../../MODULES/SCD30_module/SCD30_module.h"

void test_HAL();
void read_firmware();
void read_firmware_api();
void throw_error(uint8_t status, uint8_t index);
void test_init();
void test_get_reading();
void test_data_ready();
void read_measure_interval();

void test_SCD30_module_start(){
		uart1_hal_init();
		test_init();
		
		while(1){
			//test_HAL();
			//read_firmware();
			//read_firmware_api();
			
			test_get_reading();
			//test_data_ready();
			
			//read_measure_interval();
		
			_delay_ms(1500);
		}
	
	
}
const  uint8_t _SET_MEASUREMENT_INTERVAL[]={0x46, 0x00, 0x00, 0x09, 0x09};
void read_measure_interval(){
	uint8_t write[]={0x46, 0x00};
	uint8_t data[3];
	
	
	
	TWI_API_write_data_stop(0x61, _SET_MEASUREMENT_INTERVAL, 5);
	_delay_ms(10);
	
	TWI_API_write_data_stop(0x61, write, 2);
	_delay_ms(10);
	
	TWI_API_read_data_ack_end_nack_stop(0x61, data, 3);
	
	uart1_hal_send_message(data, 3);
	
}

void test_data_ready(){
	bool ready=SCD30_data_ready();
	if(ready==true){
		char msg[]="OK";
		uart1_hal_send_message((uint8_t*)msg, 3);
	}else{
		char msg[]="FAIL";
		uart1_hal_send_message((uint8_t*)msg, 5);
	}
}

void test_init(){
		SCD30_STATUS status=SCD30_init(2);
		if(status==SCD30_STATUS_SUCCESS){
			char msg[]="OK";
			uart1_hal_send_message((uint8_t*)msg, 3);
		}else if(status==SCD30_STATUS_TRY_AGAIN){
			char msg[]="TRY";
			uart1_hal_send_message((uint8_t*)msg, 4);
		}else{
			char msg[]="FAIL";
			uart1_hal_send_message((uint8_t*)msg, 5);
		}
}

void test_get_reading(){
	char data[20];
	uint16_t value;
	
	SCD30_STATUS status=SCD30_get_reading(&value);
	if(status==SCD30_STATUS_SUCCESS){
		sprintf(data, "%u ", value);
		uart1_hal_send_message(data, strlen(data));
	}else if(status==SCD30_STATUS_TRY_AGAIN){
		char msg[]={0xEE, 0xEE};
		uart1_hal_send_message((uint8_t*)msg, 2);
	}else{
		char msg[]={0xFF, 0xFF};
		uart1_hal_send_message((uint8_t*)msg, 2);
	}
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

void read_firmware(){
	uint8_t status;
	uint8_t data[]={0x00, 0x00, 0x00};
	
	TWI_HAL_init();
	
	status=TWI_HAL_start(0x61, WRITE);
	if(status!=TWI_CODE_SUCCESS) 
		return throw_error(status, 0);
		
	status=TWI_HAL_write_byte(0xD1);
	if(status!=TWI_CODE_SUCCESS)
		return throw_error(status, 1);
		
	status=TWI_HAL_write_byte(0x00);
	if(status!=TWI_CODE_SUCCESS)
		return throw_error(status, 2);
		
	TWI_HAL_stop();
	_delay_ms(5);
	
	status=TWI_HAL_start(0x61, READ);
	if(status!=TWI_CODE_SUCCESS)
		return throw_error(status, 3);
		
	data[0]=TWI_HAL_read_byte_ack();
	data[1]=TWI_HAL_read_byte_ack();
	data[2]=TWI_HAL_read_byte_nack();
	
	TWI_HAL_stop();
	
	uart1_hal_send_message(data, 3);
	
}

void throw_error(uint8_t status, uint8_t index){
	uint8_t msg[3];
	msg[0]=status;
	msg[1]=index;
	msg[2]=0xFF;
	uart1_hal_send_message(msg, 3);
}

void test_HAL(){
	TWI_HAL_init();
	uint8_t status=TWI_HAL_start(0x61, WRITE);
	uint8_t msg[2];
	msg[0]=status;
	msg[1]=0xff;
	uart1_hal_send_message(msg, 2);
	//if(status==0){
		//char msg[]="0 ";
		//uart1_hal_send_message((uint8_t*)msg, 3);
	//}else if(status==1){
		//char msg[]="OK";
		//uart1_hal_send_message((uint8_t*)msg, 5);
	//}else {
		//char msg[]="OK";
		//uart1_hal_send_message((uint8_t*)msg, 5);
	//}
}