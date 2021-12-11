#include "mrpp_state.h"


static void update_bodies(MRPP_STATE *state, uint8_t collectionId);
static void add_dynamic_data_types(MRPP_STATE *state, uint8_t package[]);
static uint8_t dynamic_data_type_length(uint8_t nCollections);
static uint8_t generator_header_tail(MRPP_STATE *state, uint8_t package[], uint8_t subId);

void mrpp_state_init(MRPP_STATE *state, uint8_t groupId, COLLECTION collections[], uint8_t nCollections){
    state->groupId=groupId;
    state->nCollections=nCollections;

    uint16_t startingIndex=0;
    for (uint8_t i = 0; i < nCollections; i++)
    {
        state->collections[i].samplingInterval=collections[i].samplingInterval;

        //type
        state->collections[i].type=collections[i].type;

        //Samples
        state->collections[i].nSamples=collections[i].samplings;

        //Calculate length and set starting index
        state->collections[i].startIndex=startingIndex;
        uint16_t len=collections[i].samplings*collections[i].type+COLLECTION_DATA_META_SIZE;
        state->collections[i].length=len;
        

        //calculate starting and ending body
        state->collections[i].beginsInBody=startingIndex/DR_BODY_PAYLOAD_SIZE;
        uint8_t endsInBody=(startingIndex+len-1)/DR_BODY_PAYLOAD_SIZE;

        //Simpler solution above
        state->collections[i].endsInBody=endsInBody;
        

        //set status
        state->collections[i].status=WAITING;


        startingIndex+=len;      
    }

    //Calculate lastSubId
    
    uint8_t lastSubId=(startingIndex-1)/DR_BODY_PAYLOAD_SIZE+DR_SUBID_OVERHEAD;
    state->lastSubId=lastSubId;
    
    //calculate bodies
    uint8_t nBodies=(startingIndex-1)/DR_BODY_PAYLOAD_SIZE+1;
    state->nBodies=nBodies;

    for (uint8_t i = 0; i < nBodies; i++)
    {
        state->bodies[i]=WAITING;
    }
}

uint8_t mrpp_state_get_header(MRPP_STATE *state, uint8_t package[]){
    return generator_header_tail(state, package, 0);
}

uint8_t mrpp_state_get_tail(MRPP_STATE *state, uint8_t package[]){
    return generator_header_tail(state, package, state->lastSubId);
}

static uint8_t generator_header_tail(MRPP_STATE *state, uint8_t package[], uint8_t subId){
    package[0]=subId;
    package[1]=state->lastSubId;

    //status bit
    package[2]=0;

    //N collections
    package[3]=state->nCollections;

    //add data type
    add_dynamic_data_types(state, &package[4]);

    //Calculate collection offset
    uint8_t offset=dynamic_data_type_length(state->nCollections);
    offset+=4; //subId+lastSubId+statusBit

    for (uint8_t i = 0; i < state->nCollections; i++)
    {
        package[i+offset]=state->collections[i].nSamples;
    }
    
    return offset+state->nCollections;
}

static uint8_t dynamic_data_type_length(uint8_t nCollections){
    return (nCollections/4)+1;
}

static void add_dynamic_data_types(MRPP_STATE *state, uint8_t dt[]){
    uint8_t dataTypesLen=dynamic_data_type_length(state->nCollections);

    for (uint8_t bIndex = 0; bIndex < dataTypesLen; bIndex++)
    {
        uint8_t bitArray=0x00;
        for (uint8_t index = 0; index < 4; index++)
        {
            //If there is no more collection just skip and use default 0x00
            uint8_t nCol=bIndex*4+index;
            if(nCol+1>state->nCollections) break;

            uint8_t shifts=index*2;
            switch (state->collections[nCol].type)
            {
                case T_INT8:
                    bitArray |=1 << shifts;
                    break;
                
                case T_INT16:
                    bitArray |= 2 << shifts;
                    break;

                case T_FLOAT:
                    bitArray |= 3 << shifts;
                    break;
                
                default:
                    break;
            }

        }
        //set from the end
        dt[dataTypesLen-bIndex-1]=bitArray;
        
    }
    
} 


void mrpp_state_set_collection(MRPP_STATE *state, uint8_t collectionId, uint8_t timestamp[4], uint8_t metadata[6]){
    state->collections[collectionId-1].status=DONE;

    //Copy ts
    for (uint8_t i = 0; i < 4; i++)
    {
        metadata[i]=timestamp[i];
    }

    //add sampling interval
    metadata[4]=state->collections[collectionId-1].samplingInterval>>8;
    metadata[5]=state->collections[collectionId-1].samplingInterval;


    //Update bodies
    update_bodies(state, collectionId);
}


static void update_bodies(MRPP_STATE *state, uint8_t collectionId){
    
    //update begin
    bool beginIsDone=true;
    uint8_t beginsInBody=state->collections[collectionId-1].beginsInBody;
    for (uint8_t i = 0; i < collectionId-1; i++)
    {
        if(i==collectionId-1) continue;

        if( state->collections[i].endsInBody==beginsInBody){
            if(state->collections[i].status!=DONE){
                beginIsDone=false;
                break;
            }
        }
    }

    if(beginIsDone){
        state->bodies[beginsInBody]=READY;
    }
    

    //update end
    bool endIsDone=true;
    uint8_t endsInBody=state->collections[collectionId-1].endsInBody;
    for (uint8_t i = collectionId; i < state->nCollections; i++)
    {
        if(i==collectionId-1) continue;

        if(state->collections[i].beginsInBody==endsInBody){
            if(state->collections[i].status!=DONE){
                endIsDone=false;
                break;
            }
        }
    }

    if(endIsDone){
        state->bodies[endsInBody]=READY;
    }else{
        //to handle cases where there are multiple collection in same body that starts and ends there
        state->bodies[endsInBody]=WAITING;
    }
    
    //update between
    if(endsInBody-beginsInBody<2) return;

    for (uint8_t i = beginsInBody+1; i < endsInBody; i++)
    {
        state->bodies[i]=READY;
    }
    
}

int16_t mrpp_state_is_body_ready(MRPP_STATE *state){
    int16_t bodyIndex=-1;
    for (uint8_t i = 0; i < state->nBodies; i++)
    {
        if(state->bodies[i]==READY){
            bodyIndex=i;
            break;
        }
    }
    return bodyIndex;
}

bool mrpp_state_all_bodies_sent(MRPP_STATE *state){
    bool allSent=true;

    for (uint8_t i = 0; i < state->nBodies; i++)
    {
        if(state->bodies[i]!= SENT){
            allSent=false;
        }
    }
    return allSent;
    
}

bool mrpp_state_get_ready_body(MRPP_STATE *state, int16_t bodyIndex, uint8_t *subId, uint8_t *lastSubId, uint16_t *begin, uint8_t*length){
    int16_t res=mrpp_state_is_body_ready(state);
    if(res==-1 || res!=bodyIndex ) return false;

    uint8_t readyIndex=0;
    for (uint8_t i = 0; i < state->nBodies; i++)
    {
        if(state->bodies[i]==READY){
            readyIndex=i;
            break;
        }
    }

    *subId=readyIndex+1;
    *lastSubId=state->lastSubId;

    *begin=readyIndex*DR_BODY_PAYLOAD_SIZE;
    if(readyIndex<state->nBodies-1){
        *length=DR_BODY_PAYLOAD_SIZE;
    }else{
        *length=(state->collections[state->nCollections-1].startIndex+state->collections[state->nCollections-1].length)-(state->nBodies-1)*DR_BODY_PAYLOAD_SIZE; 
    } 
    return true; 
}

void mrpp_state_set_body_sent(MRPP_STATE *state, int16_t bodyIndex){
    if(state->nBodies<=bodyIndex) return;

    state->bodies[bodyIndex]=SENT;
}


void mrpp_state_get_collection_address(MRPP_STATE *state, uint8_t collectionId, uint16_t *begin, uint16_t*length){
    *begin=state->collections[collectionId-1].startIndex;
    *length=state->collections[collectionId-1].length;
}
