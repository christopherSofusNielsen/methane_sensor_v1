/*
 * test_ADC_module.c
 *
 * Created: 18-10-2021 21:12:00
 *  Author: Mainframe
 */ 

//#define F_CPU 8000000UL
#include <xc.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#define SLAVE_ADDR 0x4D

#include "test_ADC_module.h"
#include "../../HAL/UART1/UART1_HAL.h"
#include "../../MODULES/ADC_module/ADC_module.h"
#include "../../HAL/TWI/TWI_API.h"
#include "../../HAL/TWI/TWI_HAL.h"
#include "../../util/bit_operators.h"
#include "../../HAL/PM/PM_HAL.h"

void test_calc_ppm_methane();
void read_adc();
void get_value();
void init_sampling();
void test_sampling();
void test_heater();

void test_ADC_module_start(){
	uart1_hal_init();
	TWI_HAL_init();
	while(1){
		test_calc_ppm_methane();
		//read_adc();
		//get_value();
		//init_sampling();
		//test_sampling();
		//test_heater();
		
		
		_delay_ms(1000);
	}
}

/************************************************************************/
/* Test heater                                                          */
/************************************************************************/
void test_heater(){
	uart1_hal_send_string("Start");
	_delay_ms(100);
	PM_HAL_meth_power_init();
	ADC_meth_sens_power_on(10);
	while(!ADC_meth_sens_ready()){}
	ADC_meth_sens_power_off();
	uart1_hal_send_string("Done");
}


/************************************************************************/
/* Test sampling                                                        */
/************************************************************************/
void test_sampling(){
	uint16_t data[10];
	for (uint8_t i=0; i<10; i++)
	{
		data[i]=10;
	}
	
	//Init hardware
	PM_HAL_adc_power_init();
	
	ADC_STATUS status=ADC_init_sampling(255, 5, data);
	if(status!=ADC_STATUS_SUCCESS){
		uart1_hal_send_string("FAIL");
	}
	
	ADC_start_sampling();
	while(!ADC_is_sampling_done()){}
		
	for (uint8_t i=0; i<7; i++)
	{
		char msg[20];
		sprintf(msg, " %u ", data[i]);
		uart1_hal_send_string(msg);
		_delay_ms(200);
	}
	
}

/************************************************************************/
/* Test init adc                                                        */
/************************************************************************/
void init_sampling(){
	uint16_t data[10];
	
	//Init hardware
	PM_HAL_adc_power_init();
	
	ADC_STATUS status=ADC_init_sampling(5, 10, data);
	if(status!=ADC_STATUS_SUCCESS){
		uart1_hal_send_string("FAIL");
	}else{
		uart1_hal_send_string("OK");
	}
}

/************************************************************************/
/* Test get one value                                                   */
/************************************************************************/
void get_value(){
	uint16_t ppm;
	ADC_STATUS status=ADC_get_value(&ppm);
	if(status!= ADC_STATUS_SUCCESS){
		uart1_hal_send_string("FAIL");
	}else{
		char msg[20];
		sprintf(msg, " %u ", ppm);
		uart1_hal_send_string(msg);
	}
}



/************************************************************************/
/* Calculate PPM                                                        */
/************************************************************************/
void test_calc_ppm_methane(){
	char data[20];
	ADC_set_conf_parameters(5.00, 2500.00, 1);
	uint16_t value=calc_ppm_methane(2000);
	sprintf(data, " %u ", value);
	uart1_hal_send_string(data);
}




void read_adc(){
	uint8_t data[20];
	
	TWI_HAL_init();
	
	TWI_API_read_data_ack_end_nack_stop(SLAVE_ADDR, data, 2);
	
	uart1_hal_send_message(data, 2);
}