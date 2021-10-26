/*
 * test_ADC_module.c
 *
 * Created: 18-10-2021 21:12:00
 *  Author: Mainframe
 */ 

#define F_CPU 8000000UL
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

void test_calc_ppm_methane();
void read_adc();

void test_ADC_module_start(){
	uart1_hal_init();
	//char msg[]="Init ";
	//uart0_hal_send_message((uint8_t *)msg, strlen(msg));
	
	while(1){
		//test_calc_ppm_methane();
		read_adc();
		_delay_ms(1000);
	}
}

void test_calc_ppm_methane(){
	char data[20];
	uint16_t value=calc_ppm_methane(2000);
	sprintf(data, " %u ", value);
	uart1_hal_send_message((uint8_t *)data, strlen(data));
}

void read_adc(){
	uint8_t data[20];
	
	TWI_HAL_init();
	
	TWI_API_read_data_ack_end_nack_stop(SLAVE_ADDR, data, 2);
	
	uart1_hal_send_message(data, 2);
}