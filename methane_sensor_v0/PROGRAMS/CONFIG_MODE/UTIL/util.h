/*
 * util.h
 *
 * Created: 09-11-2021 18:10:19
 *  Author: Mainframe
 */ 


#ifndef UTIL_H_
#define UTIL_H_

#include <xc.h>
#include <stdbool.h>

bool get_parameter(const char src[], char par[], uint8_t parNo);
uint8_t count_parameters(const char src[]);
uint16_t par_index(const char src[], uint8_t parNo);



#endif /* UTIL_H_ */