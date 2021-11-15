/*
 * TWI_HAL.h
 *
 * Created: 14-10-2021 08:44:07
 *  Author: Mainframe
 */ 


#ifndef TWI_HAL_H_
#define TWI_HAL_H_

#include <xc.h>
//#define F_CPU 8000000	

typedef enum {
	WRITE=0,
	READ=1
}TWI_HAL_START_TYPE;

#define F_SCL 100000

#define  TWI_TWBR 32

#define TWI_CODE_SUCCESS 0x00


void TWI_HAL_init();
uint8_t TWI_HAL_start(uint8_t addr, TWI_HAL_START_TYPE type);
uint8_t TWI_HAL_repeated_start(uint8_t addr);
uint8_t TWI_HAL_write_byte(uint8_t data);
uint8_t TWI_HAL_read_byte_ack();
uint8_t TWI_HAL_read_byte_nack();
void TWI_HAL_stop();







#endif /* TWI_HAL_H_ */