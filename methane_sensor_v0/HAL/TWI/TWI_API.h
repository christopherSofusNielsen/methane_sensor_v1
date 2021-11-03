/*
 * TWI_API.h
 *
 * Created: 15-10-2021 15:31:21
 *  Author: Mainframe
 */ 


#ifndef TWI_API_H_
#define TWI_API_H_

#include <xc.h>

uint8_t TWI_API_write_data(uint8_t slaveAddr, const uint8_t data[], uint8_t length);
uint8_t TWI_API_write_data_stop(uint8_t slaveAddr, const uint8_t data[], uint8_t length);
uint8_t TWI_API_read_data_ack_end_nack_stop(uint8_t slaveAddr, uint8_t data[], uint8_t length);
uint8_t TWI_API_repeat_read_data_stop(uint8_t slaveAddr, uint8_t data[], uint8_t length);

#endif /* TWI_API_H_ */