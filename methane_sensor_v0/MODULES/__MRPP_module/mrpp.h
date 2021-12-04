/*
 * mrpp.c
 *
 *  Author: Christopher S. Nielsen
 */ 

#ifndef MRPP_H
#define MRPP_H

#include <stdint.h>
#include <stdbool.h>
#include "util/mrpp_state.h"
#include "util/mrpp_data.h"

/************************************************************************/
/* Test purpose only                                                    */
/************************************************************************/
#ifdef __TEST_MODE
extern MRPP_STATE state;
extern uint8_t _groupId;
#endif


/************************************************************************
* Initializes the protocol for a new sampling process 
* -> COLLECTION collections[]: collection configuration parameters that will be used
* -> uint8_t nCollections: number of collections
* -> uint8_t groupId: Id for the new sampling process
* -> Returns a updated groupId for use on next call
************************************************************************/
uint8_t MRPP_init_group(COLLECTION collections[], uint8_t nCollections, uint8_t groupId);

/************************************************************************
* Adds samples to a collection
* -> uint8_t collectionId: The id of the collections the samples belongs to
* -> uint8_t timestamp[]: Timestamp for the first sample
* -> uint16_t values[]: the samples, the number of samples must be same as configured
************************************************************************/
void MRPP_add_collection_data_INT16(uint8_t collectionId, uint8_t timestamp[], uint16_t values[]);

/************************************************************************
* Returns true if a body is ready to be sent
* -> int16_t *bodyIndex: The bodyIndex there is ready will be sat here
************************************************************************/
bool MRPP_is_body_package_ready(int16_t *bodyIndex);

/************************************************************************
* Gets the body requested, formatted as package to be sent
* -> int16_t bodyIndex: The requested bodyIndex
* -> uint8_t package[]: Pointer to where to store the package data
* -> uint8_t *package_length: The length of the package will be stored here
* -> Returns true on success, false if the requested index is not ready
************************************************************************/
bool MRPP_get_ready_body_package(int16_t bodyIndex, uint8_t package[], uint8_t *package_length);

/************************************************************************
* Sets the body as sent
* -> int16_t bodyIndex: The index of the body that was sent
************************************************************************/
void MRPP_set_body_sent(int16_t bodyIndex);

/************************************************************************
* Gets the header package
* -> uint8_t package[]: Pointer to where to store the package data
* -> uint8_t *package_length: The length of the package will be stored here
************************************************************************/
void MRPP_get_header_package(uint8_t package[], uint8_t *package_length);

/************************************************************************
* Gets the tail package
* -> uint8_t package[]: Pointer to where to store the package data
* -> uint8_t *package_length: The length of the package will be stored here
************************************************************************/
void MRPP_get_tail_package(uint8_t package[], uint8_t *package_length);

/************************************************************************/
/* Return true if all body packages is sent                             */
/************************************************************************/
bool MRPP_all_body_package_sent();

/************************************************************************
* Validates the total size of the collections
* -> COLLECTION collections[]: collection configuration parameters that will be used
* -> uint8_t nCollections: Number of collections
* -> Returns true if the size fits in the buffer
************************************************************************/
bool MRPP_validate_collections_space(COLLECTION collections[], uint8_t nCollections);


#endif