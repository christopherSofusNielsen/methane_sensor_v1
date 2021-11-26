/*
 * to_float.h
 *
 * Created: 18-10-2021 08:04:50
 *  Author: Christopher S. Nielsen
 */ 


#ifndef TO_FLOAT_H_
#define TO_FLOAT_H_

#include <xc.h>

typedef union {
	uint8_t bytes[4];
	float f;
} to_float;



#endif /* TO_FLOAT_H_ */