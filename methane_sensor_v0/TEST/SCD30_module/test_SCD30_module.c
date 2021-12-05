/*
 * test_SCD30_module.c
 *
 * Created: 14-10-2021 08:42:54
 *  Author: Christopher S. Nielsen
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


static void test_sampling_all_values();
static void test_sampling_only_co2();
static void test_read_all_values();
static void test_calc_validate_SI();
static void test_get_reading();

void test_SCD30_module_start(){
		uart1_hal_init();
		PM_HAL_SCD30_power_init();
		TWI_HAL_init();
		
			
		while(1){
			//test_calc_validate_SI();
			//test_read_all_values();
			//test_sampling_all_values();
			//test_sampling_only_co2();
			test_get_reading();
			
			_delay_ms(3000);
		}
}

/************************************************************************/
/* Test calc and validation of SIs                                      */
/************************************************************************/
static void test_calc_validate_SI(){
	uart1_hal_send_string("Calc and validation test");
	uint16_t lowest_si;
	uint16_t highest_si;
	bool res;
	
	res=SCD30_calc_validate_SI(4, 20, 20, &lowest_si, &highest_si);
	if (res && lowest_si==4 && highest_si==20)
	{
		uart1_hal_send_string("OK 1");
	}else{
		uart1_hal_send_string("FAIL 1");
	}
	
	res=SCD30_calc_validate_SI(4, 22, 20, &lowest_si, &highest_si);
	if (res==false)
	{
		uart1_hal_send_string("OK 2");
	}else{
		uart1_hal_send_string("FAIL 2");
	}
	
	res=SCD30_calc_validate_SI(4, 4, 8, &lowest_si, &highest_si);
	if (res && lowest_si==4 && highest_si==8)
	{
		uart1_hal_send_string("OK 3");
	}else{
		uart1_hal_send_string("FAIL 3");
	}
	
	res=SCD30_calc_validate_SI(4, 8, 0, &lowest_si, &highest_si);
	if (res && lowest_si==4 && highest_si==8)
	{
		uart1_hal_send_string("OK 4");
	}else{
		uart1_hal_send_string("FAIL 4");
	}
}

/************************************************************************/
/* Test the read function                                               */
/************************************************************************/
static void test_read_all_values(){
	SCD30_STATUS status;
	uint16_t co2, temp, humidity;
	
	uart1_hal_send_string("Test read of values");
	
	status=SCD30_sensor_on();
	if(status!=SCD30_STATUS_SUCCESS){
		uart1_hal_send_string("Failed ");
		return;
	}
	
	status=read_all_values(&co2, &temp, &humidity);
	if(status!=SCD30_STATUS_SUCCESS){
		uart1_hal_send_string("Failed ");
		return;
	}
	
	char msg[30];
	sprintf(msg, "%u, %u, %u", co2, temp, humidity);
	uart1_hal_send_string(msg);
}



/************************************************************************/
/* Test sampling of all values                                          */
/************************************************************************/
static void test_sampling_all_values(){
	uint16_t co2[6], temp[3], humidity[3];
	
	uart1_hal_send_string("Test sampling of c02, temp and humidity");
	SCD30_STATUS status=SCD30_sensor_on();
	if(status!=SCD30_STATUS_SUCCESS){
		uart1_hal_send_string("FAIL 1");
		return;
	}
	
	//Initialize samplings
	SCD30_init_c02_sampling(3, 6, co2);
	SCD30_init_temp_sampling(6, 3, temp);
	SCD30_init_humidity_sampling(6, 3, humidity);
	
	bool res=SCD30_start_sampling();
	if(!res){
		uart1_hal_send_string("FAIL 2");
		return;
	}
	
	//Wait to finish
	while(!SCD30_is_sampling_done()){};
	
	SCD30_deinit_sampling();
	
	//Send readings
	char msg[40];
	sprintf(msg, "%u, %u, %u, %u, %u, %u", co2[0], co2[1], co2[2], co2[3], co2[4], co2[5], co2[6]);
	uart1_hal_send_string(msg);
	
	sprintf(msg, "%u, %u, %u", temp[0], temp[1], temp[2]);
	uart1_hal_send_string(msg);
	
	sprintf(msg, "%u, %u, %u", humidity[0], humidity[1], humidity[2]);
	uart1_hal_send_string(msg);
}

/************************************************************************/
/* Test sampling of co2 only                                            */
/************************************************************************/
static void test_sampling_only_co2(){
	uint16_t co2[6];
	
	uart1_hal_send_string("Test sampling of c02 only");
	SCD30_STATUS status=SCD30_sensor_on();
	if(status!=SCD30_STATUS_SUCCESS){
		uart1_hal_send_string("FAIL 1");
		return;
	}
	
	//Initialize samplings
	SCD30_init_c02_sampling(3, 6, co2);
	
	bool res=SCD30_start_sampling();
	if(!res){
		uart1_hal_send_string("FAIL 2");
		return;
	}
	
	//Wait to finish
	while(!SCD30_is_sampling_done()){};
	
	SCD30_deinit_sampling();
	
	//Send readings
	char msg[40];
	sprintf(msg, "%u, %u, %u, %u, %u, %u", co2[0], co2[1], co2[2], co2[3], co2[4], co2[5], co2[6]);
	uart1_hal_send_string(msg);
}

/************************************************************************/
/* Test get reading                                                     */
/************************************************************************/
static void test_get_reading(){
	SCD30_STATUS status;
	uint16_t co2, temp, humidity;
	
	uart1_hal_send_string("Test get reading");
	
	status=SCD30_sensor_on();
	if(status!=SCD30_STATUS_SUCCESS){
		uart1_hal_send_string("Failed ");
		return;
	}
	
	status=SCD30_get_reading(&co2, &temp, &humidity);
	if(status!=SCD30_STATUS_SUCCESS){
		uart1_hal_send_string("Failed ");
		return;
	}
	
	char msg[30];
	sprintf(msg, "%u, %u, %u", co2, temp, humidity);
	uart1_hal_send_string(msg);
}







