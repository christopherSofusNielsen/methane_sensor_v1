/*
 * test_RTC_module.c
 *
 * Created: 26/10/2021 16.13.18
 *  Author: ccons
 */ 

#define F_CPU 8000000UL

#include <xc.h>
#include <util/delay.h>
#include <stdbool.h>

#include "test_RTC_module.h"
#include "../../HAL/TWI/TWI_HAL.h"
#include "../../HAL/UART1/UART1_HAL.h"
#include "../../HAL/TWI/TWI_API.h"
#include "../../util/bit_operators.h"

#define SLAVE_ADDR 0x51

void test_read_clk();
void test_write_clk();

void test_RTC_module_start(){
	uart1_hal_init();
	
	
	TWI_HAL_init();
	

	while(1){
		//test_read_clk();
		test_write_clk();
		
		_delay_ms(1500);
	}
}

void test_read_clk(){
	uint8_t CMD_READ_CLK[]={0x0D};
	uint8_t data[10];
	
	
	//Set register pointer
	TWI_API_write_data_stop(SLAVE_ADDR, CMD_READ_CLK, 1);
	
	//Read value
	TWI_API_read_data_ack_end_nack_stop(SLAVE_ADDR, data, 1);
	
	uart1_hal_send_message(data, 1);
	
}

void test_write_clk(){
	//Set pull up
	set_bit(PORTB, 0);
	set_bit(PORTB, 1);
	
	
	uint8_t CMD_SET_CLK[]={0x0D, 0x83};
	uint8_t CMD_READ_CLK[]={0x0D};
	uint8_t data[10];
	
	
	//Set register pointer
	TWI_API_write_data_stop(SLAVE_ADDR, CMD_SET_CLK, 2);
	
	//Set register pointer
	TWI_API_write_data_stop(SLAVE_ADDR, CMD_READ_CLK, 1);
	
	
	//Read value
	TWI_API_read_data_ack_end_nack_stop(SLAVE_ADDR, data, 1);
	
	uart1_hal_send_message(data, 1);
	
}

