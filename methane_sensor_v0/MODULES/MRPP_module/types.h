/*
 * mrpp.h
 *
 *  Author: Christopher S. Nielsen
 */ 
#ifndef MRPP_TYPES_H
#define MRPP_TYPES_H

#include <stdint.h>

typedef enum {
    T_INT8=1,
    T_INT16=2,
    T_FLOAT=4
} MEASURE_TYPE;

typedef struct COLLECTION
{
    uint16_t samplings;
    uint16_t samplingInterval;
    MEASURE_TYPE type;
} COLLECTION;

#endif