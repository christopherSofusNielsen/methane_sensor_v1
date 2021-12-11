#include "mrpp.h"
#include <stdio.h>

MRPP_STATE state;

uint8_t MRPP_init_group(COLLECTION collections[], uint8_t nCollections, uint8_t groupId){
    mrpp_state_init(&state, groupId, collections, nCollections);

    return groupId++;
}


void MRPP_add_collection_data_INT16(uint8_t collectionId, uint8_t timestamp[], uint16_t values[]){
    uint8_t metadata[6];
    uint16_t begin;
    uint16_t length;
    mrpp_state_set_collection(&state, collectionId, timestamp, metadata);
    mrpp_state_get_collection_address(&state, collectionId, &begin, &length);

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

    bool res=mrpp_state_get_ready_body(&state, bodyIndex, &subId, &lastSubId, &begin, &length);
    
    if(!res) return false;

    *package_length=length+DR_BODY_PACKAGE_META_SIZE;

    //set meta
    package[0]=state.groupId;
    package[1]=subId;
    package[2]=lastSubId;

    //Merge data
    mrpp_data_get(&package[3], begin, length);

    return true;
}

void MRPP_set_body_sent(int16_t bodyIndex){
    mrpp_state_set_body_sent(&state, bodyIndex);
}

void MRPP_get_header_package(uint8_t package[], uint8_t *package_length){
    package[0]=state.groupId;
    *package_length=mrpp_state_get_header(&state, &package[1]);
    *package_length+=1;
}

void MRPP_get_tail_package(uint8_t package[], uint8_t *package_length){
    package[0]=state.groupId;
    *package_length=mrpp_state_get_tail(&state, &package[1]);
    *package_length+=1;
}

bool MRPP_all_body_package_sent(){
    return mrpp_state_all_bodies_sent(&state);
}

bool MRPP_validate_collections_space(COLLECTION collections[], uint8_t nCollections){
    //Check number of collections
    uint8_t headerSize=HEADER_TAIL_META_SIZE+nCollections*DR_HEADER_COLLECTION_META_SIZE;
    if(headerSize>MAX_PAYLOAD_SIZE) return false;

    //Check buffer size
    uint16_t size=0;
    for (uint8_t i = 0; i < nCollections; i++)
    {
        size+=collections[i].samplings*collections[i].type+COLLECTION_DATA_META_SIZE;
    }
    
    if(size<=BODY_BUFFER_DATA_SIZE) return true;
    return false;
}

