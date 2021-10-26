/*
 * UART1_HAL.h
 *
 * Created: 04-10-2021 18:14:37
 *  Author: Mainframe
 */ 


#ifndef UART1_HAL_H_
#define UART1_HAL_H_

#include <xc.h>
#include <stdbool.h>

#define UART1_TX_BUFF_LENGTH 200
#define UART1_RX_BUFF_LENGTH 100
#define UART1_END_CHAR_1 '\r'
#define UART1_END_CHAR_2 '\n'


void uart1_hal_init();
void uart1_hal_send_message(uint8_t msg[], uint8_t length);
bool uart1_hal_message_ready();
uint8_t uart1_hal_read_message(uint8_t msg[]);
void uart1_hal_clear_rx_buffer();




#endif /* UART1_HAL_H_ */