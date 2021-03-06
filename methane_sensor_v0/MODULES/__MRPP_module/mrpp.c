/*
 * mrpp.h
 *
 *  Author: Christopher S. Nielsen
 */ 


#include "mrpp.h"
#include <stdio.h>

static MRPP_STATE state;


uint8_t MRPP_init_group(COLLECTION collections[], uint8_t nCollections, uint8_t groupId){
    mrpp_state_init(&state, groupId, collections, nCollections);
    return groupId+1; //Increment groupId
}

void MRPP_add_collection_data_INT16(uint8_t collectionId, uint8_t timestamp[], uint16_t values[]){
    uint8_t metadata[6];
    uint16_t begin;
    uint16_t length;
	
	//update state, generate meta-data for collection (ts+sampling interval)
    mrpp_state_set_collection(&state, collectionId, timestamp, metadata);
	
	//get address to where to store the meta-data + samples in the buffer
    mrpp_state_get_collection_address(&state, collectionId, &begin, &length);

	//Store meta-data and samples in buffer 
    mrpp_data_add_int16(metadata, values, begin, length);
}

bool MRPP_is_body_package_ready(int16_t *bodyIndex){
    *bodyIndex = mrpp_state_is_body_ready(&state);
    return *bodyIndex>-1;
}

bool MRPP_get_ready_body_package(int16_t bodyIndex, uint8_t package[], uint8_t *package_length){
    uint8_t subId;
    uint8_t lastSubId;
    uint16_t begin;
    uint8_t length;

	//get body overhead (subId & lastSubId) and the address of the data in the buffer
    bool res=mrpp_state_get_ready_body(&state, bodyIndex, &subId, &lastSubId, &begin, &length);
    
    if(!res) return false;

    *package_length=length+DR_BODY_PACKAGE_META_SIZE;

    //set overhead data
    package[0]=state.groupId;
    package[1]=subId;
    package[2]=lastSubId;

    //Merge samples
    mrpp_data_get(&package[3], begin, length);

    return true;
}

void MRPP_set_body_sent(int16_t bodyIndex){
    mrpp_state_set_body_sent(&state, bodyIndex);
}

void MRPP_get_header_package(uint8_t package[], uint8_t *package_length){
	//set overhead
    package[0]=state.groupId;
	
	//Generate and get header package on the fly
    *package_length=mrpp_state_get_header(&state, &package[1]);
    *package_length+=1;
}

void MRPP_get_tail_package(uint8_t package[], uint8_t *package_length){
	//set overhead
    package[0]=state.groupId;
	
	//Generate and get header package on the fly
    *package_length=mrpp_state_get_tail(&state, &package[1]);
    *package_length+=1;
}

bool MRPP_all_body_package_sent(){
    return mrpp_state_all_bodies_sent(&state);
}

bool MRPP_validate_collections_space(COLLECTION collections[], uint8_t nCollections){
    uint16_t size=0;
	//Calculate the sum of collection data + meta data (ts & sampling interval)
    for (uint8_t i = 0; i < nCollections; i++)
    {
        size+=collections[i].samplings*collections[i].type+COLLECTION_DATA_META_SIZE;
    }
    
	//Compare to buffer size
    if(size<=BODY_BUFFER_DATA_SIZE) return true;
    return false;
}

