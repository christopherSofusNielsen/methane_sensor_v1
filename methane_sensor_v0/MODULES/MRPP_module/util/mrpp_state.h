
#ifndef MRPP_STATE_H
#define MRPP_STATE_H

#include <stdint.h>
#include <stdbool.h>
#include "../types.h"
#include "../params.h"


typedef enum {
    WAITING,
    READY,
    SENT,
    DONE,
} DR_STATUS;


typedef struct __COLLECTION_STATE
{
    uint16_t startIndex;
    uint16_t length;
    MEASURE_TYPE type;
    uint16_t samplingInterval;
    uint8_t beginsInBody;
    uint8_t endsInBody;
    DR_STATUS status;
}__COLLECTION_STATE;


typedef struct MRPP_STATE
{
    uint8_t groupId;
    uint8_t lastSubId;
    uint8_t nCollections;
    __COLLECTION_STATE collections[12];
    uint8_t nBodies;
    DR_STATUS bodies[MAX_BODIES];

} MRPP_STATE;





void mrpp_state_init(MRPP_STATE *state, uint8_t groupId, COLLECTION collections[], uint8_t nCollections);
uint8_t mrpp_state_get_header(MRPP_STATE *state, uint8_t package[]);
uint8_t mrpp_state_get_tail(MRPP_STATE *state, uint8_t package[]);
void mrpp_state_set_collection(MRPP_STATE *state, uint8_t collectionId, uint8_t timestamp[4], uint8_t metadata[6]);
int16_t mrpp_state_is_body_ready(MRPP_STATE *state);
bool mrpp_state_all_bodies_sent(MRPP_STATE *state);
bool mrpp_state_get_ready_body(MRPP_STATE *state, int16_t bodyIndex, uint8_t *subId, uint8_t *lastSubId, uint16_t *begin, uint8_t *length);
void mrpp_state_set_body_sent(MRPP_STATE *state, int16_t bodyIndex);
void mrpp_state_get_collection_address(MRPP_STATE *state, uint8_t collectionId, uint16_t *begin, uint16_t*length);
#endif