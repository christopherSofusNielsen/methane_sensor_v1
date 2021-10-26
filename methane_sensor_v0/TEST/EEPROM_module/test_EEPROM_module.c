/*
 * test_EEPROM_module.c
 *
 * Created: 18-10-2021 13:27:43
 *  Author: Mainframe
 */ 

#define F_CPU 8000000UL
#include <xc.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#include "test_EEPROM_module.h"
#include "../../HAL/UART0/UART0_HAL.h"
#include "../../MODULES/EEPROM_module/EEPROM_module.h"

void clear_lora_parameters();
void set_deveui();
void test_set_get_parameter();

void test_EEPROM_module_start(){
	uart0_hal_init();
	char msg[]="Init ";
	uart0_hal_send_message(msg, strlen(msg));
	
	while(1){
		//clear_lora_parameters();
		//set_deveui();
		//test_set_get_parameter();
		_delay_ms(5000);
	}
}

void test_set_get_parameter(){
	char space[]=" - ";
	char data[40];
	char deveui[]="0004A30B00F4547A";
	char appeui[]="70B3D57ED003F844";
	char appkey[]="B88AD6D25A3B27C69A01F74C53F9A179";
	
	EM_set_deveui(deveui);
	EM_set_appeui(appeui);
	EM_set_appkey(appkey);
	
	EM_get_deveui(data);
	uart0_hal_send_message(data, strlen(data));
	_delay_ms(200);
	uart0_hal_send_message(space, strlen(space));
	_delay_ms(200);
	
	EM_get_appeui(data);
	uart0_hal_send_message(data, strlen(data));
	_delay_ms(200);
	uart0_hal_send_message(space, strlen(space));
	_delay_ms(200);
	
	EM_get_appkey(data);
	uart0_hal_send_message(data, strlen(data));
	_delay_ms(200);
	uart0_hal_send_message(space, strlen(space));
	_delay_ms(200);
	
}

void set_deveui(){
	char deveui[]="0004A30B00F4547A";
	char deveui_read[20];
	
	EM_set_deveui(deveui);
	if(!EM_has_deveui()){
		char msg[]="FAIL";
		uart0_hal_send_message(msg, strlen(msg));
		return;
	}
	EM_get_deveui(deveui_read);
	uart0_hal_send_message(deveui_read, strlen(deveui_read));
}

void clear_lora_parameters(){
	EM_clear_lora_prameters();
	uint8_t status=EM_has_deveui();
	if(status){
		char msg[]="T ";
		uart0_hal_send_message(msg, strlen(msg));
	}else{
		char msg[]="F ";
		uart0_hal_send_message(msg, strlen(msg));
	}
}