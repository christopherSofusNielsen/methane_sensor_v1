/*
 * UART1_HAL.h
 *
 * Created: 04-10-2021 18:14:37
 *  Author: Christopher S. Nielsen
 */ 


#ifndef UART1_HAL_H_
#define UART1_HAL_H_

#include <xc.h>
#include <stdbool.h>

#define UART1_TX_BUFF_LENGTH 200
#define UART1_RX_BUFF_LENGTH 150
#define UART1_END_CHAR_1 '\n'
#define UART1_END_CHAR_2 '\r'


void uart1_hal_init();
void uart1_hal_send_message(uint8_t msg[], uint8_t length);
void uart1_hal_send_string(const char msg[]);
bool uart1_hal_message_ready();
bool uart1_hal_rx_full();
uint8_t uart1_hal_read_message(uint8_t msg[]);
uint8_t uart1_hal_read_message_as_str(char msg[]);
void uart1_hal_clear_rx_buffer();
//void uart0_hal_send_break(uint8_t followUpByte);




#endif /* UART1_HAL_H_ */