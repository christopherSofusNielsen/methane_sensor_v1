/*
 * test_EEPROM_module.c
 *
 * Created: 18-10-2021 13:27:43
 *  Author: Christopher S. Nielsen
 */ 

#define F_CPU 8000000UL
#include <xc.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#include "test_EEPROM_module.h"
#include "../../HAL/UART1/UART1_HAL.h"
#include "../../MODULES/EEPROM_module/EEPROM_module.h"
#include "../../MODULES/MRPP_module/types.h"

void clear_lora_parameters();
void set_deveui();
void test_set_get_parameter();
static void set_get_float();
static void set_get_collections();

void test_EEPROM_module_start(){
	uart1_hal_init();
	uart1_hal_send_string("Init ");
	
	while(1){
		//clear_lora_parameters();
		//set_deveui();
		//test_set_get_parameter();
		//set_get_float();
		//set_get_collections();
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
	uart1_hal_send_string(data);
	_delay_ms(200);
	uart1_hal_send_string(space);
	_delay_ms(200);
	
	EM_get_appeui(data);
	uart1_hal_send_string(data);
	_delay_ms(200);
	uart1_hal_send_string(space);;
	_delay_ms(200);
	
	EM_get_appkey(data);
	uart1_hal_send_string(data);
	_delay_ms(200);
	uart1_hal_send_string(space);
	_delay_ms(200);
	
}

static void set_get_collections(){
	char msg[20];
	
	COLLECTION cols[]={
		{
			.samplingInterval=1,
			.samplings=2,
			.type=T_INT16
		},
		{
			.samplingInterval=3,
			.samplings=4,
			.type=T_INT8
		}
	};
	
	COLLECTION colsRead[2];
	
	EM_set_collections((const void *) cols, 10);
	EM_get_collections((void *) colsRead, 10);
	
	sprintf(msg, "%u %u %u ", colsRead[0].samplingInterval, colsRead[0].samplings, colsRead[0].type);
	uart1_hal_send_string(msg);
	sprintf(msg, "%u %u %u ", colsRead[1].samplingInterval, colsRead[1].samplings, colsRead[1].type);
	uart1_hal_send_string(msg);
	
}

static void set_get_float(){
	float ppm, rrl, vcc;
	char msg[30];
	
	EM_set_ppmfactor(12.34);
	EM_set_RRL(5.67);
	EM_set_Vcc(8.9);
	_delay_ms(30);
	EM_get_ppmfactor(&ppm);
	EM_get_RRL(&rrl);
	EM_get_Vcc(&vcc);
	
	sprintf(msg, "%.2f %.2f %.2f ", ppm, rrl, vcc);
	uart1_hal_send_string(msg);
}

void set_deveui(){
	char deveui[]="0004A30B00F4547A";
	char deveui_read[20];
	
	EM_set_deveui(deveui);
	if(!EM_has_deveui()){
		uart1_hal_send_string("FAIL");
		return;
	}
	EM_get_deveui(deveui_read);
	uart1_hal_send_string(deveui_read);
}

void clear_lora_parameters(){
	EM_clear_lora_prameters();
	uint8_t status=EM_has_deveui();
	if(status){
		uart1_hal_send_string("T ");
	}else{
		uart1_hal_send_string("F ");
	}
}