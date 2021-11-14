/*
 * test_lora_module.c
 *
 * Created: 04-10-2021 16:51:38
 *  Author: Mainframe
 */ 

#include <util/delay.h>

#include "test_lora_module.h"
#include "../../HAL/UART0/UART0_HAL.h"
#include "../../HAL/UART1/UART1_HAL.h"
#include "../../HAL/RN2483/RN2483_HAL.h"
#include "../../HAL/TC2/TC2_HAL.h"
#include "../../MODULES/LORA_module/lora_module.h"

static void test_join_network();
static void test_send_uplink();
static void test_send_uplink_block();
static void test_put_to_sleep();
static void test_wake_up();
static void test_reset();
static void test_forward_msg();



void test_LORA_module_start(){
	uart1_hal_init();
	uart0_hal_init();
	rn2483_init();
	TC2_HAL_init();
	
	while(1){
		uart1_hal_send_string("Starting: ");
		
		test_join_network();
		//test_send_uplink();
		//test_send_uplink_block();
		//test_put_to_sleep();
		//test_wake_up();
		//test_reset();
		//test_forward_msg();
		
		_delay_ms(3000);
	}
}

static void test_join_network(){
	char deveui[]="0004A30B00F4547A";
	char appeui[]="70B3D57ED003F844";
	char appkey[]="B88AD6D25A3B27C69A01F74C53F9A179";
	
	
	LM_STATUS status=LM_join_network(deveui, appeui, appkey);
	if(status==LM_STATUS_SUCCESS){
		uart1_hal_send_string("OK ");
	}else if(status==LM_STATUS_TRY_AGAIN){
		uart1_hal_send_string("TRY AGAIN ");
	}else if(status==LM_STATUS_CONF_ERR){
		uart1_hal_send_string("CONF_ERR ");
	}else{
		uart1_hal_send_string("FAIL ");
	}
}

static void test_send_uplink(){
	uint8_t data[]={0xAA, 0xBB, 0xFF};
	
	LM_STATUS status=LM_send_uplink(data, 3);
	if(status==LM_STATUS_SUCCESS){
		uart1_hal_send_string("OK ");
		}else if(status==LM_STATUS_TRY_AGAIN){
		uart1_hal_send_string("TRY AGAIN ");
		}else if(status==LM_STATUS_MAC_ERR){
		uart1_hal_send_string("MAC_ERR ");
		}else if(status==LM_STATUS_INV_DATA_LEN){
		uart1_hal_send_string("inv length ");
		}else{
		uart1_hal_send_string("FAIL ");
		return;
	}
}

static void test_send_uplink_block(){
	uint8_t data[]={0xAA, 0xBB, 0xFF};
	
	LM_STATUS status=LM_send_uplink(data, 3);
	if(status==LM_STATUS_SUCCESS){
		uart1_hal_send_string("OK ");
	}else if(status==LM_STATUS_TRY_AGAIN){
		uart1_hal_send_string("TRY AGAIN ");
	}else if(status==LM_STATUS_MAC_ERR){
		uart1_hal_send_string("MAC_ERR ");
	}else if(status==LM_STATUS_INV_DATA_LEN){
		uart1_hal_send_string("inv length ");
	}else{
		uart1_hal_send_string("FAIL ");
		return;
	}
	
	status=LM_send_uplink(data, 3);
	if(status!=LM_STATUS_TRY_AGAIN){
		uart1_hal_send_string("FAIL ");
		return;
	}
	
	while(!LM_is_free()){};
	uart1_hal_send_string("DONE ");
}

static void test_put_to_sleep(){
	LM_STATUS status=LM_put_to_sleep();
	if(status==LM_STATUS_SUCCESS){
		uart1_hal_send_string("OK ");	
	}else{
		uart1_hal_send_string("FAIL ");
	}
}


static void test_wake_up(){
	LM_STATUS status=LM_wake_up();
	if(status==LM_STATUS_SUCCESS){
		uart1_hal_send_string("OK ");
	}else{
		uart1_hal_send_string("FAIL ");
	}
}

static void test_reset(){
	LM_STATUS status=LM_reset_module();
	if(status==LM_STATUS_SUCCESS){
		uart1_hal_send_string("OK ");
	}else{
		uart1_hal_send_string("FAIL ");
		
	}
}

static void test_forward_msg(){
	char res[30];
	LM_forward_msg("sys get hweui", res);
	uart1_hal_send_string(res);
}
