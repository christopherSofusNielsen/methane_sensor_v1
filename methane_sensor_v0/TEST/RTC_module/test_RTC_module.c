/*
 * test_RTC_module.c
 *
 * Created: 26/10/2021 16.13.18
 *  Author: ccons
 */ 

#include <xc.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>

#include "test_RTC_module.h"
#include "../../HAL/TWI/TWI_HAL.h"
#include "../../HAL/UART1/UART1_HAL.h"
#include "../../HAL/TWI/TWI_API.h"
#include "../../util/bit_operators.h"
#include "../../MODULES/RTC_module/RTC_module.h"
#include "../../HAL/PM/PM_HAL.h"

#define SLAVE_ADDR 0x51

//static void test_read_clk();
//static void test_write_clk();
//static void test_read_time();
//static void test_dec_to_bcd();
//static void test_bcd_to_dec();
static void test_format_dt_to_ts();
static void test_get_current_time();
static void test_set_current_time();
static void test_set_wake_up();
static void test_clk_out();
static void sendISO(Datetime dt, char msg[]);



void test_RTC_module_start(){
	PM_HAL_SCD30_power_init();
	PM_HAL_SCD30_power(true);
	PM_HAL_adc_power_init();
	PM_HAL_adc_power(false);
	
	uart1_hal_init();
	TWI_HAL_init();
	
	//Set pull up
	//set_bit(PORTB, 0);
	//set_bit(PORTB, 1);

	while(1){
		
		test_clk_out();
		//test_get_current_time();
		//test_set_current_time();
		//test_set_wake_up();
		
		//test_dec_to_bcd();
		//test_bcd_to_dec();
		//test_format_dt_to_ts();
		
		_delay_ms(5000);
	}
}

static void test_clk_out(){
	RTC_STATUS status=RTC_set_clock_out(1);
	if(status==RTC_STATUS_SUCCESS){
		uart1_hal_send_string("OK ");
	}else{
		uart1_hal_send_string("FAIL ");
	}
}

static void test_get_current_time(){
	
	Datetime dt;
	
	RTC_STATUS status= RTC_get_current_time(&dt);
	if(status != RTC_STATUS_SUCCESS){
		uart1_hal_send_string("Fail ");
		return;
	}
	
	char msg[50];
	sendISO(dt, msg);
	uart1_hal_send_string(msg);
}

static void test_set_current_time(){
	Datetime dt={
		.second=0,
		.minute=0,
		.hour=0,
		.day=10,
		.month=10,
		.year=21
	};
	
	RTC_STATUS status= RTC_set_current_time(dt);
	if(status!=RTC_STATUS_SUCCESS) {
		uart1_hal_send_string(" Fail ");
		return;
	}else{
		uart1_hal_send_string(" OK ");
	}
}


static void test_set_wake_up(){
	RTC_STATUS status;
	Datetime dt={
		.second=0,
		.minute=59,
		.hour=9,
		.day=3,
		.month=11,
		.year=21
	};
	
	status = RTC_set_current_time(dt);
	if(status != RTC_STATUS_SUCCESS){
		uart1_hal_send_string("Failed set time ");
		return;
	}
	
	status=RTC_set_wake_up_interrupt(1);
	if(status != RTC_STATUS_SUCCESS){
		uart1_hal_send_string("Failed set intr ");
		return;
	}
	
	uart1_hal_send_string("Sleep ");
	_delay_ms(200);
	PM_HAL_enter_power_down();
	uart1_hal_send_string("Awake ");
	_delay_ms(200);
	
	status=RTC_clear_wake_up_interrupt();
	if(status != RTC_STATUS_SUCCESS){
		uart1_hal_send_string("Failed to clear ");
		return;
	}
	uart1_hal_send_string("Done ");
}

//static void test_dec_to_bcd(){
	//DatetimeBCD bcd;
	//Datetime dt={
		//.second=11,
		//.minute=22,
		//.hour=33,
		//.day=44,
		//.month=55,
		//.year=66
	//};
	//
	//datetime_to_BCD(dt, &bcd);
	//uart1_hal_send_message(&bcd.second, 6);
//}

//static void test_bcd_to_dec(){
	//DatetimeBCD bcd={
		//.second=0x11,
		//.minute=0x22,
		//.hour=0x33,
		//.day=0x44,
		//.month=0x55,
		//.year=0x66
	//};
	//Datetime dt;
	//
	//BCD_to_datetime(bcd, &dt);
	//uart1_hal_send_message(&dt.second, 6);
//}

static void test_format_dt_to_ts(){
	uint8_t ts[4];
	Datetime dt={
		.second=11,
		.minute=04,
		.hour=13,
		.day=8,
		.month=11,
		.year=21
	};
	
	RTC_datetime_to_ts(dt, ts);
	uart1_hal_send_message(ts, 4);
}

/************************************************************************/
/* Helper functions                                                     */
/************************************************************************/
static void sendISO(Datetime dt, char msg[]){
	sprintf(msg, "%04u-%02u-%02uT%02u:%02u:%02u", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);
}

/************************************************************************/
/* Direct debug functions                                               */
/************************************************************************/
//static void test_read_clk(){
	//uint8_t CMD_READ_CLK[]={0x0D};
	//uint8_t data[10];
	//uint8_t status1;
	//uint8_t status2;
	//
	//
	////Set register pointer
	//status1=TWI_API_write_data_stop(SLAVE_ADDR, CMD_READ_CLK, 1);
	//
	////Read value
	//status2=TWI_API_read_data_ack_end_nack_stop(SLAVE_ADDR, data, 1);
	//
	//data[1]=status1;
	//data[2]=status2;
	//
	//uart1_hal_send_message(data, 3);
	//
//}
//
//static void test_write_clk(){
	////Set pull up
	//set_bit(PORTB, 0);
	//set_bit(PORTB, 1);
	//
	//
	//uint8_t CMD_SET_CLK[]={0x0D, 0x83};
	//uint8_t CMD_READ_CLK[]={0x0D};
	//uint8_t data[10];
	//
	//
	////Set register pointer
	//TWI_API_write_data_stop(SLAVE_ADDR, CMD_SET_CLK, 2);
	//
	////Set register pointer
	//TWI_API_write_data_stop(SLAVE_ADDR, CMD_READ_CLK, 1);
	//
	//
	////Read value
	//TWI_API_read_data_ack_end_nack_stop(SLAVE_ADDR, data, 1);
	//
	//uart1_hal_send_message(data, 1);
	//
//}
//
//static void test_read_time(){
	//uint8_t CMD_SET_POINTER[]={0x02};
	//uint8_t data[9];
	//uint8_t status1;
	//uint8_t status2;
	//
	//status1=TWI_API_write_data(SLAVE_ADDR, CMD_SET_POINTER, 1);
	//status2=TWI_API_repeat_read_data_stop(SLAVE_ADDR, data, 7);
	//
	//
	//data[7]=status1;
	//data[8]=status2;
	//
	//
	//uart1_hal_send_message(data, 9);
	//_delay_ms(3000);
//}



