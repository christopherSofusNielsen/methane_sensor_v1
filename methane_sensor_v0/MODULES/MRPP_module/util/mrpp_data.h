/*
 * mrpp.h
 *
 *  Author: Christopher S. Nielsen
 */ 

#ifndef MRPP_DATA_H
#define MRPP_DATA_H

#include <stdint.h>
#include "../params.h"

#ifdef __TEST_MODE
extern uint8_t storage[800];
#endif

void mrpp_data_add_int16(uint8_t metadata[6], uint16_t values[], uint16_t begin, uint16_t length);
void mrpp_data_get(uint8_t data[], uint16_t begin, uint16_t length);
#endif