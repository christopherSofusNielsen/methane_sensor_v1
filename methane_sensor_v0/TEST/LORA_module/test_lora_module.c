/*
 * test_lora_module.c
 *
 * Created: 04-10-2021 16:51:38
 *  Author: Mainframe
 */ 

#include <util/delay.h>

#include "test_lora_module.h"
#include "../../HAL/UART0/UART0_HAL.h"
#include "../../HAL/RN2483/RN2483_HAL.h"
#include "../../MODULES/LORA_module/lora_module.h"
#include "../../HAL/TC2/TC2_HAL.h"

void test_join_network();
void test_send_uplink();
void test_send_uplink_block();
void test_put_to_sleep();
void test_wake_up();
void test_reset();
void test_forward_msg();


void test_lora_module_start(){
	
	uart0_hal_init();
	rn2483_init();
	TC2_HAL_init();
	
	while(1){
		uart0_hal_send_string("Starting: ");
		//test_join_network();
		//test_send_uplink();
		//test_send_uplink_block();
		//test_put_to_sleep();
		//test_wake_up();
		//test_reset();
		test_forward_msg();
		_delay_ms(3000);
	}
}

void test_wake_up(){
	LM_STATUS status=LM_wake_up();
	if(status==LM_STATUS_SUCCESS){
		uart0_hal_send_string("OK ");
	}else if(status==LM_STATUS_TRY_AGAIN){
		uart0_hal_send_string("TRY AGAIN ");
	}else if(status==LM_STATUS_ERROR){
		uart0_hal_send_string("ERROR ");
	}else if(status==LM_STATUS_CONF_ERR){
		uart0_hal_send_string("CONF_ERR ");
	}else{
		uart0_hal_send_string("FAIL ");
	}
}

void test_put_to_sleep(){
	LM_STATUS status=LM_put_to_sleep();
	if(status==LM_STATUS_SUCCESS){
		uart0_hal_send_string("OK ");
	}else if(status==LM_STATUS_TRY_AGAIN){
		uart0_hal_send_string("TRY AGAIN ");
	}else if(status==LM_STATUS_ERROR){
		uart0_hal_send_string("ERROR ");
	}else if(status==LM_STATUS_CONF_ERR){
		uart0_hal_send_string("CONF_ERR ");
	}else{
		uart0_hal_send_string("FAIL ");
	}
}

void test_send_uplink(){
	uint8_t data[]={0xAA, 0xBB, 0xFF};
	
	LM_STATUS status=LM_send_uplink(data, 3);
	if(status==LM_STATUS_SUCCESS){
		uart0_hal_send_string("OK ");
	}else if(status==LM_STATUS_TRY_AGAIN){
		uart0_hal_send_string("TRY AGAIN ");
	}else if(status==LM_STATUS_ERROR){
		uart0_hal_send_string("ERROR ");
	}else if(status==LM_STATUS_CONF_ERR){
		uart0_hal_send_string("CONF_ERR ");
	}else{
		uart0_hal_send_string("FAIL ");
	}
}

void test_send_uplink_block(){
	uint8_t data[]={0xAA, 0xBB, 0xFF};
	
	LM_STATUS status=LM_send_uplink(data, 3);
	if(status!=LM_STATUS_SUCCESS){
		uart0_hal_send_string("FAIL ");
		return;
	}
	
	status=LM_send_uplink(data, 3);
	if(status!=LM_STATUS_TRY_AGAIN){
		uart0_hal_send_string("FAIL ");
		return;
	}
	
	while(LM_send_uplink(data, 3)==LM_STATUS_TRY_AGAIN){};
	
}


void test_join_network(){
	char deveui[]="0004A30B00F4547A";
	char appeui[]="70B3D57ED003F844";
	char appkey[]="B88AD6D25A3B27C69A01F74C53F9A179";
	
	
	LM_STATUS status=LM_join_network(deveui, appeui, appkey);
	if(status==LM_STATUS_SUCCESS){
		uart0_hal_send_string("OK ");
	}else if(status==LM_STATUS_TRY_AGAIN){
		uart0_hal_send_string("TRY AGAIN ");
	}else if(status==LM_STATUS_ERROR){
		uart0_hal_send_string("ERROR ");
	}else if(status==LM_STATUS_CONF_ERR){
		uart0_hal_send_string("CONF_ERR ");
	}else{
		uart0_hal_send_string("FAIL ");
	}
}

void test_reset(){
	LM_STATUS status=LM_reset_module();
	if(status==LM_STATUS_SUCCESS){
		uart0_hal_send_string("OK ");
	}else{
		uart0_hal_send_string("FAIL ");
		
	}
}

void test_forward_msg(){
	char res[30];
	LM_forward_msg("sys get hweui", res);
	uart0_hal_send_string(res);
}
