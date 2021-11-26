/*
 * UART0_HAL.c
 *
 * Created: 04-10-2021 14:23:34
 *  Author: Christopher S. Nielsen
 */ 

#define UART0_BAUDRATE (8000000/(8*57600))-1

#include <util/delay.h>
#include <xc.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>


#include "UART0_HAL.h"
#include "../../util/bit_operators.h"

uint8_t tx_buffer[UART0_TX_BUFF_LENGTH];
uint8_t tx_buffer_data_len=0;
uint8_t tx_buffer_cursor=0;

uint8_t rx_buffer[UART0_RX_BUFF_LENGTH];
uint8_t rx_buffer_data_len=0;
uint8_t rx_buffer_has_message=0;

static void copy_buffer(uint8_t msg[]);

void uart0_hal_init(){
	set_bit(UCSR0B, 4); //Enable Rx
	set_bit(UCSR0B, 3); //Enable Tx
	set_bit(UCSR0B, 7); //interrupt Rx complete
	set_bit(UCSR0B, 6); //interrupt Tx complete
	set_bit(UCSR0A, 1); //Set double speed
	UCSR0C = 0b00000110; //Async, none parity, 1 stop bit, 8 databit
	UBRR0=UART0_BAUDRATE; //set baudrate
	sei();
}


void uart0_hal_send_message(uint8_t msg[], uint8_t length){
	while(tx_buffer_data_len!=0){};//wait to write if buffer not empty
	for (uint8_t i=0; i<length; i++)
	{
		tx_buffer[i]=msg[i];
	}
	
	tx_buffer_data_len=length;
	tx_buffer_cursor=1;
	UDR0=tx_buffer[0];
}

void uart0_hal_send_string(const char msg[]){
	while(tx_buffer_data_len!=0){};//wait to write if buffer not empty
	uint8_t len=strlen(msg);
	for (uint8_t i=0; i<len; i++)
	{
		tx_buffer[i]=(uint8_t)msg[i];
	}
	
	tx_buffer_data_len=len;
	tx_buffer_cursor=1;
	UDR0=tx_buffer[0];
}

bool uart0_hal_message_ready(){
	return rx_buffer_has_message>0;
}

uint8_t uart0_hal_read_message(uint8_t msg[]){
	copy_buffer(msg);
	uint8_t len= rx_buffer_data_len-2;
	uart0_hal_clear_rx_buffer();
	return len;
}


uint8_t uart0_hal_read_message_as_str(char msg[]){
	copy_buffer((uint8_t *) msg);
	uint8_t len= rx_buffer_data_len-2;
	msg[len]='\0';
	uart0_hal_clear_rx_buffer();
	return len;
}

void uart0_hal_clear_rx_buffer(){
	rx_buffer_has_message=0;
	rx_buffer_data_len=0;
}

void uart0_hal_send_break(uint8_t followUpByte){
	set_bit(DDRD, 1); //Set TX0 (PD1) as output
	clear_bit(UCSR0B, 3); //Disable Tx
	clear_bit(PORTD, 1); //pull PD1 low
	_delay_ms(2); //Wait one ms
	set_bit(UCSR0B, 3); //Enable Tx
	
	uint8_t data[1];
	data[0]=followUpByte;
	uart0_hal_send_message(data, 1);
}

static void copy_buffer(uint8_t msg[]){
	for (uint8_t i=0; i<rx_buffer_data_len-2; i++)
	{
		msg[i]=rx_buffer[i];
	}
}


/************************************************************************/
/* Write data complete vect                                             */
/************************************************************************/
ISR(USART0_TX_vect){
	if(tx_buffer_cursor<tx_buffer_data_len){
		UDR0=tx_buffer[tx_buffer_cursor++];
	}else{
		tx_buffer_cursor=0;
		tx_buffer_data_len=0;
	}
}

/************************************************************************/
/* Read data complete vect                                              */
/************************************************************************/
ISR(USART0_RX_vect){
	if(rx_buffer_has_message==0 && rx_buffer_data_len<UART0_RX_BUFF_LENGTH){
		rx_buffer[rx_buffer_data_len++]=UDR0;
	}
	if(rx_buffer_data_len>2 && rx_buffer[rx_buffer_data_len-1]==UART0_END_CHAR_1 && rx_buffer[rx_buffer_data_len-2]==UART0_END_CHAR_2){
		rx_buffer_has_message=1;
	}
}
