/*
 * CRC8.h
 *
 * Created: 15-10-2021 15:19:39
 *  Author: Mainframe
 */ 


#ifndef CRC8_H_
#define CRC8_H_

#include <xc.h>

#define CRC_POLYNOMIAL 0x31
#define CRC_INIT_VALUE 0xFF

uint8_t calc_crc8(uint8_t data[], uint8_t length);



#endif /* CRC8_H_ */