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

#include "test_ADC_module.h"
#include "../../HAL/UART0/UART0_HAL.h"
#include "../../MODULES/ADC_module/ADC_module.h"

void test_calc_ppm_methane();

void test_ADC_module_start(){
	uart0_hal_init();
	char msg[]="Init ";
	uart0_hal_send_message((uint8_t *)msg, strlen(msg));
	
	while(1){
		test_calc_ppm_methane();
		_delay_ms(1000);
	}
}

void test_calc_ppm_methane(){
	char data[20];
	uint16_t value=calc_ppm_methane(2000);
	sprintf(data, " %u ", value);
	uart0_hal_send_message((uint8_t *)data, strlen(data));
}