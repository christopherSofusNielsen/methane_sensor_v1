/*
 * CRC8.c
 *
 * Created: 15-10-2021 15:19:27
 *  Author: Christopher S. Nielsen
 */ 

#include "CRC8.h"

uint8_t calc_crc8(uint8_t data[], uint8_t length){
	    uint8_t remainder=CRC_INIT_VALUE;

	    for (uint8_t byte = 0; byte < length; byte++)
	    {
		    remainder ^= data[byte];

		    for (uint8_t i = 8; i >0; --i)
		    {
			    if(remainder & 0x80){
				    remainder=(remainder << 1) ^ CRC_POLYNOMIAL;
				    }else{
				    remainder=(remainder << 1);
			    }
		    }
	    }
	    return remainder;
}