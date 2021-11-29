/*
 * test_MRPP_module.c
 *
 * Created: 05-11-2021 13:50:58
 *  Author: Christopher S. Nielsen
 */ 

#include <util/delay.h>

#include "test_MRPP_module.h"
#include "../../HAL/UART1/UART1_HAL.h"
#include "../../MODULES/MRPP_module/mrpp.h"
#include "helpers/util.h"


static void test_report_example();
static void test_groupid();

void test_MRPP_module_start(){
	uart1_hal_init();
	
	while(1){
		//uart1_hal_send_string("Working ");
		test_report_example();
		//test_groupid();
		
		
		_delay_ms(10000);
	}
}

static void test_report_example(){
	COLLECTION collections[]={
		{.samplingInterval=30, .samplings=30, .type=T_INT16},
		{.samplingInterval=120, .samplings=10, .type=T_INT16},
	};
	uint8_t fake_ts[]={0xAA, 0xBB, 0xCC, 0xDD};
	uint16_t data[30];
	
	 MRPP_init_group(collections, 2, 0);
	
	 //Add data for collection 1
	 util_fill_data(data, 0xAAAA, 30 );
	 MRPP_add_collection_data_INT16(1, fake_ts, data);

	 //Add data for collection 2
	 util_fill_data(data, 0xBBBB, 10 );
	 MRPP_add_collection_data_INT16(2, fake_ts, data);
		
	//Variables
	int16_t bodyIndex;
	uint8_t package[51];
	uint8_t package_length;
	char str[300]={'\0'};
	
	//Header
	uart1_hal_send_string("Header");
	MRPP_get_header_package(package, &package_length);
	util_2_hex_str(str, package, package_length);
	uart1_hal_send_string(str);
	_delay_ms(1000);
	
	 //Wite bodies as long they are available
	 while (MRPP_is_body_package_ready(&bodyIndex))
	 {

		 //Write body
		 uart1_hal_send_string("Body");
		 MRPP_get_ready_body_package(bodyIndex, package, &package_length);
		 util_2_hex_str(str, package, package_length);
		 uart1_hal_send_string(str);
		 _delay_ms(1000);

		 //Set package as sent
		 MRPP_set_body_sent(bodyIndex);
	 }
	 
	 //Make sure all packages is done
	 if(!MRPP_all_body_package_sent()){
		 uart1_hal_send_string("ERROR, not all bodies is sent");
		 return;
	 }
	
	//Write tail
	uart1_hal_send_string("Tail");
	MRPP_get_tail_package(package, &package_length);
	util_2_hex_str(str, package, package_length);
	util_2_hex_str(str, package, package_length);
	uart1_hal_send_string(str);
	_delay_ms(1000);
}

static void test_groupid(){
	COLLECTION collections[]={
		{.samplingInterval=30, .samplings=30, .type=T_INT16},
		{.samplingInterval=120, .samplings=10, .type=T_INT16},
	};
	
	uint8_t newGroupId=MRPP_init_group(collections, 2, 255);
	if(newGroupId!=0){
		uart1_hal_send_string("Fail");
	}else{
		uart1_hal_send_string("Success");
	}
}


