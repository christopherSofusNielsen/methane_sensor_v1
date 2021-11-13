/*
 * test_ADC_module.c
 *
 * Created: 18-10-2021 21:12:00
 *  Author: Mainframe
 */ 


#include <xc.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#define SLAVE_ADDR 0x4D

#include "test_ADC_module.h"
#include "../../HAL/UART1/UART1_HAL.h"
#include "../../MODULES/ADC_module/ADC_module.h"
#include "../../HAL/TWI/TWI_HAL.h"
#include "../../util/bit_operators.h"
#include "../../HAL/PM/PM_HAL.h"
#include "../../HAL/TC0/TC0_HAL.h"

static void get_value();
static void test_sampling();
static void test_heater();
//static void test_calc_ppm_methane();



void test_ADC_module_start(){
	uart1_hal_init();
	TWI_HAL_init();
	PM_HAL_meth_power_init();
	PM_HAL_adc_power_init();
	
	//Set pull up
	set_bit(PORTB, 0);
	set_bit(PORTB, 1);
	
	while(1){
		//test_heater();
		//test_sampling();
		//get_value();

		_delay_ms(1000);
	}
}

/************************************************************************/
/* Test heater                                                          */
/************************************************************************/
static void test_heater(){
	uart1_hal_send_string("Start");

	ADC_meth_sens_power_on(10);
	while(!ADC_meth_sens_ready()){}
	ADC_meth_sens_power_off();
	uart1_hal_send_string("Done");
}


/************************************************************************/
/* Test sampling                                                        */
/************************************************************************/
static void test_sampling(){
	uint16_t data[5];
	for (uint8_t i=0; i<5; i++)
	{
		data[i]=10;
	}
	
	//Init hardware
	PM_HAL_adc_power_init();
	
	ADC_STATUS status=ADC_init_sampling(2, 5, data);
	if(status!=ADC_STATUS_SUCCESS){
		uart1_hal_send_string("FAIL");
	}
	
	ADC_start_sampling();
	while(!ADC_is_sampling_done()){}
		
	for (uint8_t i=0; i<5; i++)
	{
		char msg[20];
		sprintf(msg, " %u ", data[i]);
		uart1_hal_send_string(msg);
	}
	
}



/************************************************************************/
/* Test get one value                                                   */
/************************************************************************/
static void get_value(){
	uart1_hal_send_string("Get value");
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
//static void test_calc_ppm_methane(){
//char data[20];
//ADC_set_conf_parameters(5.00, 2500.00, 1);
//uint16_t value=calc_ppm_methane(2000);
//sprintf(data, " %u ", value);
//uart1_hal_send_string(data);
//}