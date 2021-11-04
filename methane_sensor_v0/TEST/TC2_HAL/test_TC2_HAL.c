/*
 * test_TC2_HAL.c
 *
 * Created: 04-11-2021 20:27:13
 *  Author: Mainframe
 */ 

#include <xc.h>
#include <util/delay.h>

#include "test_TC2_HAL.h"
#include "../../HAL/UART1/UART1_HAL.h"
#include "../../HAL/TC2/TC2_HAL.h"

static void test_sub();
static void test_timing();

static void f1();
static void f2();


static uint8_t f1Cnt=0;
static uint8_t f2Cnt=0;

void test_TC2_HAL_start(){
	uart1_hal_init();
	
	while(1){
		//test_sub();
		test_timing();
		
		_delay_ms(1000);
	}
}

static void test_timing(){
	uart1_hal_send_string("Start ");
	f1Cnt=0;
	
	TC2_HAL_init();
	int8_t p1=TC2_HAL_subscribe(&f1);
	
	while(f1Cnt<5){};
	
	uart1_hal_send_string("Stop ");
	TC2_HAL_cancel(p1);
}


static void test_sub_cancel(){
	uart1_hal_send_string("Start ");
	_delay_ms(100);
	f1Cnt=0;
	f2Cnt=0;
	
	TC2_HAL_init();
	int8_t p1=TC2_HAL_subscribe(&f1);
	int8_t p2=TC2_HAL_subscribe(&f2);
	
	uint8_t lastF1=0;
	uint8_t lastF2=0;
	
	while(1){
		if(f1Cnt>lastF1){
			lastF1=f1Cnt;
			uart1_hal_send_string("f1 ");
						_delay_ms(50);
		}
		
		if(f2Cnt>lastF2 && f2Cnt%2==0){
			lastF2=f2Cnt;
			uart1_hal_send_string("f2 ");
			_delay_ms(50);
		}
		
		if(f2Cnt>4){
			TC2_HAL_cancel(p2);
		}
		
		if(f1Cnt>6){
			TC2_HAL_cancel(p1);
			break;
		}
	
	}
	
	
	
	
}


static void f1(){
	f1Cnt++;
}

static void f2(){
	f2Cnt++;
}