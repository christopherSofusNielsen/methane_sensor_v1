/*
 * util.h
 *
 * Created: 05-11-2021 14:27:31
 *  Author: Mainframe
 */ 


#ifndef UTIL_H_
#define UTIL_H_

#include <xc.h>

void util_fill_data(uint16_t arr[], uint16_t data, uint16_t length);
void util_2_hex_str(char str[], uint8_t data[], uint8_t length);

#endif /* UTIL_H_ */