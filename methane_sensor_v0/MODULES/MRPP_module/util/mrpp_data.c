/*
 * mrpp.h
 *
 *  Author: Christopher S. Nielsen
 */ 

#include "mrpp_data.h"

uint8_t storage[BODY_BUFFER_DATA_SIZE];

void mrpp_data_add_int16(uint8_t metadata[6], uint16_t values[], uint16_t begin, uint16_t length){
    
    //add meta
    for (uint8_t i = begin; i < begin+COLLECTION_DATA_META_SIZE; i++)
    {
        storage[i]=metadata[i-begin];
    }
    length-=COLLECTION_DATA_META_SIZE;

    //add values  
    uint16_t valuesLength=length/2;
    const uint16_t offset_MSB=begin+COLLECTION_DATA_META_SIZE;
    const uint16_t offset_LSB=begin+COLLECTION_DATA_META_SIZE+1;
    for (uint16_t i = 0; i < valuesLength; i++)
    {
        storage[offset_MSB + i*2]=values[i]>>8;
        storage[offset_LSB + i*2]=values[i];
    }

}

void mrpp_data_get(uint8_t data[], uint16_t begin, uint16_t length){
    for (uint16_t i = 0; i < length; i++)
    {
        data[i]=storage[begin+i];
    }
    
}