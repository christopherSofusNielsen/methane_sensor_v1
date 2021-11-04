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

void test_join_network();
void test_send_uplink();
void test_put_to_sleep();
void test_wake_up();


void test_lora_module_start(){
	
	uart0_hal_init();
	rn2483_init();
	
	
	while(1){
		uart0_hal_send_string("Starting: ");
		test_join_network();
		//test_send_uplink();
		//test_put_to_sleep();
		//test_wake_up();
		_delay_ms(3000);
	}
}

void test_wake_up(){
	LM_STATUS status=LM_wake_up();
	if(status==LM_STATUS_SUCCESS){
		char msg[]="OK";
		uart0_hal_send_message((uint8_t*)msg, 3);
		}else if(status==LM_STATUS_TRY_AGAIN){
		char msg[]="TRY";
		uart0_hal_send_message((uint8_t*)msg, 4);
		}else{
		char msg[]="FAIL";
		uart0_hal_send_message((uint8_t*)msg, 5);
	}
}

void test_put_to_sleep(){
	LM_STATUS status=LM_put_to_sleep();
	if(status==LM_STATUS_SUCCESS){
		char msg[]="OK";
		uart0_hal_send_message((uint8_t*)msg, 3);
		}else if(status==LM_STATUS_TRY_AGAIN){
		char msg[]="TRY";
		uart0_hal_send_message((uint8_t*)msg, 4);
		}else{
		char msg[]="FAIL";
		uart0_hal_send_message((uint8_t*)msg, 5);
	}
}

void test_send_uplink(){
	uint8_t data[]={0xAA, 0xBB, 0xFF};
	
	LM_STATUS status=LM_send_uplink(data, 3);
	if(status==LM_STATUS_SUCCESS){
		char msg[]="OK";
		uart0_hal_send_message((uint8_t*)msg, 3);
	}else if(status==LM_STATUS_TRY_AGAIN){
		char msg[]="TRY";
		uart0_hal_send_message((uint8_t*)msg, 4);
	}else if(status==LM_STATUS_ERROR){
		char msg[]="ERR";
		uart0_hal_send_message((uint8_t*)msg, 4);
	}else{
		char msg[]="FAIL";
		uart0_hal_send_message((uint8_t*)msg, 5);
	}
}

void test_join_network(){
	LM_STATUS status=LM_join_network();
	if(status==LM_STATUS_SUCCESS){
		char msg[]="OK";
		uart0_hal_send_message((uint8_t*)msg, 3);
		}else if(status==LM_STATUS_TRY_AGAIN){
		char msg[]="TRY";
		uart0_hal_send_message((uint8_t*)msg, 4);
		}else{
		char msg[]="FAIL";
		uart0_hal_send_message((uint8_t*)msg, 5);
	}
}
