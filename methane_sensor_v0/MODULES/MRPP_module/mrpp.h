
#ifndef MRPP_H
#define MRPP_H

#include <stdint.h>
#include <stdbool.h>
#include "util/mrpp_state.h"
#include "util/mrpp_data.h"

#ifdef __TEST_MODE
extern MRPP_STATE state;
extern uint8_t _groupId;
#endif



void MRPP_init_group(COLLECTION collections[], uint8_t nCollections);
void MRPP_add_collection_data_INT16(uint8_t collectionId, uint8_t timestamp[], uint16_t values[]);
bool MRPP_is_body_package_ready(int16_t *bodyIndex);
bool MRPP_get_ready_body_package(int16_t bodyIndex, uint8_t package[], uint8_t *package_length);
void MRPP_set_body_sent(int16_t bodyIndex);
void MRPP_get_header_package(uint8_t package[], uint8_t *package_length);
void MRPP_get_tail_package(uint8_t package[], uint8_t *package_length);
bool MRPP_all_body_package_sent();
bool MRPP_validate_collections_space(COLLECTION collections[], uint8_t nCollections);


#endif