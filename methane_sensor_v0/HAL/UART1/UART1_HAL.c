/*
 * UART1_HAL.c
 *
 * Created: 04-10-2021 18:15:25
 *  Author: Christopher S. Nielsen
 */ 

#define UART1_BAUDRATE (8000000/(8*57600))-1

#include <util/delay.h>
#include <xc.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>


#include "UART1_HAL.h"
#include "../../util/bit_operators.h"

static void copy_buffer(uint8_t msg[]);

static uint8_t tx_buffer[UART1_TX_BUFF_LENGTH];
static uint16_t tx_buffer_data_len=0;
static uint16_t tx_buffer_cursor=0;

static uint8_t rx_buffer[UART1_RX_BUFF_LENGTH];
static uint8_t rx_buffer_data_len=0;
static uint8_t rx_buffer_has_message=0;

void uart1_hal_init(){
	set_bit(UCSR1B, 4); //Enable Rx
	set_bit(UCSR1B, 3); //Enable Tx
	set_bit(UCSR1B, 7); //interrupt Rx complete
	set_bit(UCSR1B, 6); //interrupt Tx complete
	set_bit(UCSR1A, 1); //Set double speed
	UCSR1C = 0b00000110; //Async, none parity, 1 stop bit, 8 databit
	UBRR1=UART1_BAUDRATE; //set baudrate
	sei();
}
void uart1_hal_send_string(const char msg[]){
	while(tx_buffer_data_len!=0){};//wait to write if buffer not empty
	uint16_t len=strlen(msg);
	for (uint16_t i=0; i<len; i++)
	{
		tx_buffer[i]=(uint8_t)msg[i];
	}
	tx_buffer[len]=UART1_END_CHAR_2;
	tx_buffer[len+1]=UART1_END_CHAR_1;
	
	tx_buffer_data_len=len+2;
	tx_buffer_cursor=1;
	UDR1=tx_buffer[0];
}

void uart1_hal_send_message(uint8_t msg[], uint8_t length){
	while(tx_buffer_data_len!=0){};//wait to write if buffer not empty
	for (uint16_t i=0; i<length; i++)
	{
		tx_buffer[i]=msg[i];
	}
	
	tx_buffer_data_len=length;
	tx_buffer_cursor=1;
	UDR1=tx_buffer[0];
}

bool uart1_hal_message_ready(){
	return rx_buffer_has_message>0;
}

bool uart1_hal_rx_full(){
	return rx_buffer_data_len>=UART1_RX_BUFF_LENGTH-1;
}

uint8_t uart1_hal_read_message(uint8_t msg[]){
	copy_buffer(msg);
	uint8_t len= rx_buffer_data_len-2;
	uart1_hal_clear_rx_buffer();
	return len;
}

uint8_t uart1_hal_read_message_as_str(char msg[]){
	copy_buffer((uint8_t *)msg);
	uint8_t len= rx_buffer_data_len-2;
	msg[len]='\0';
	uart1_hal_clear_rx_buffer();
	return len;
}

void uart1_hal_clear_rx_buffer(){
	rx_buffer_has_message=0;
	rx_buffer_data_len=0;
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
ISR(USART1_TX_vect){
	if(tx_buffer_cursor<tx_buffer_data_len){
		UDR1=tx_buffer[tx_buffer_cursor++];
	}else{
		tx_buffer_cursor=0;
		tx_buffer_data_len=0;
	}
}

/************************************************************************/
/* Read data complete vect                                              */
/************************************************************************/
ISR(USART1_RX_vect){
	if(rx_buffer_has_message==0 && rx_buffer_data_len<UART1_RX_BUFF_LENGTH){
		rx_buffer[rx_buffer_data_len++]=UDR1;
	}
	if(rx_buffer_data_len>2 && rx_buffer[rx_buffer_data_len-1]==UART1_END_CHAR_1 && rx_buffer[rx_buffer_data_len-2]==UART1_END_CHAR_2){
		rx_buffer_has_message=1;
	}
}
