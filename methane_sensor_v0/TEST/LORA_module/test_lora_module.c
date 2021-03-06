/*
 * test_lora_module.c
 *
 * Created: 04-10-2021 16:51:38
 *  Author: Christopher S. Nielsen
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
static void power_test();



void test_LORA_module_start(){
	uart1_hal_init();
	uart0_hal_init();
	rn2483_init();
	TC2_HAL_init();
	
	while(1){
		//uart1_hal_send_string("Starting: ");
		
		//test_join_network();
		//test_send_uplink();
		//test_send_uplink_block();
		//test_put_to_sleep();
		//test_wake_up();
		//test_reset();
		//test_forward_msg();
		power_test();
		
		_delay_ms(3000);
	}
}

static void test_join_network(){
	char deveui[]="70B3D57ED0047B7D";
	char appeui[]="0000000000000000";
	char appkey[]="EA11A4D6D368D5CDE102B0491B40A494";
	
	
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

static void power_test(){
	char deveui[]="70B3D57ED0047B7D";
	char appeui[]="0000000000000000";
	char appkey[]="EA11A4D6D368D5CDE102B0491B40A494";
	uint8_t data[51];
	
	//Load data
	for (uint8_t i=0; i<51; i++)
	{
		data[i]=0xAA;
	}
	uart1_hal_send_string("Start LORA power test");
	
	
	//Join network
	uart1_hal_send_string("Join network");
	LM_STATUS status=LM_join_network(deveui, appeui, appkey);
	if(status==LM_STATUS_SUCCESS){
		uart1_hal_send_string("OK ");
	}else if(status==LM_STATUS_TRY_AGAIN){
		uart1_hal_send_string("TRY AGAIN ");
		return;
	}else if(status==LM_STATUS_CONF_ERR){
		uart1_hal_send_string("CONF_ERR ");
		return;
	}else{
		uart1_hal_send_string("FAIL ");
		return;
	}
	
	//Send 3 uplink
	
	for (uint8_t i=0; i<2; i++)
	{
		uart1_hal_send_string("Transmitting uplink");
		LM_STATUS status=LM_send_uplink(data, 51);
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
		
		while (!LM_is_free());
	}
	
	uart1_hal_send_string("Done, power off");
	
	LM_put_to_sleep();
	_delay_ms(2000);
	
	uart1_hal_send_string("End LORA power test");
	
}
