/*
 * MAINPG.h
 *
 * Created: 05-11-2021 19:51:35
 *  Author: Christopher S. Nielsen
 */ 


#ifndef MAINPG_H_
#define MAINPG_H_



/************************************************************************/
/* Mapping of sample routine to collection                              */
/************************************************************************/
//#define S0_CO2 0
//#define S1_METH 1
//#define S1_CO2 2
//#define S2_METH 3
//#define S2_CO2 4
//#define S3_METH 5
//#define S3_CO2 6
#define S0_CO2 0
#define S0_TEMP 1
#define S0_HUM 2
#define S1_METH 3
#define S1_CO2 4
#define S1_TEMP 5
#define S1_HUM 6
#define S2_METH 7
#define S2_CO2 8
#define S2_TEMP 9
#define S2_HUM 10
#define S3_METH 11
#define S3_CO2 12
#define S3_TEMP 13
#define S3_HUM 14

/************************************************************************/
/* Mapping of sample routine to collection ID                           */
/************************************************************************/
//#define S0_ID_CO2 1
//#define S1_ID_METH 2
//#define S1_ID_CO2 3
//#define S2_ID_METH 4
//#define S2_ID_CO2 5
//#define S3_ID_METH 6
//#define S3_ID_CO2 7
#define S0_ID_CO2 1
#define S0_ID_TEMP 2
#define S0_ID_HUM 3
#define S1_ID_METH 4
#define S1_ID_CO2 5
#define S1_ID_TEMP 6
#define S1_ID_HUM 7
#define S2_ID_METH 8
#define S2_ID_CO2 9
#define S2_ID_TEMP 10
#define S2_ID_HUM 11
#define S3_ID_METH 12
#define S3_ID_CO2 13
#define S3_ID_TEMP 14
#define S3_ID_HUM 15

void MAINPG_start();

/************************************************************************/
/* States main-program state machine                                    */
/************************************************************************/
typedef enum{
	MAINPG_INIT_HW,
	MAINPG_READ_EEPROM,
	
	MAINPG_LORA_JOIN_NETWORK,
	MAINPG_LORA_JOIN_SUCCESS,
	MAINPG_LORA_JOIN_TRY_AGAIN,
	MAINPG_CONF_ERR,
	MAINPG_LORA_WAKEUP,
	
	MAINPG_INIT_RTC,
	MAINPG_SET_WAKEUP,
	MAINPG_CLEAR_WAKEUP,
	
	MAINPG_INIT_MODULES,
	
	MAINPG_INIT_MRPP,
	MAINPG_SEND_HEADER,
	
	MAINPG_STAGE_0,
	MAINPG_STAGE_1,
	MAINPG_STAGE_2,
	MAINPG_STAGE_3,
	
	MAINPG_SEND_DATA,
	MAINPG_SEND_ALL_DATA,
	MAINPG_SEND_TAIL,
	
	MAINPG_SETUP_SLEEP,
	MAINPG_SLEEP,
	
	MAINPG_PAYLOAD_INV_LEN,
	MAINPG_FATAL_ERROR,
	MAINPG_END
} MAINPG_STATES;

/************************************************************************/
/* Status codes from stage-state machine                                */
/************************************************************************/
typedef enum{
	STAGE_RUNNING,
	STAGE_DONE,
	STAGE_ERROR,
	STAGE_FATAL_ERROR
} STAGE_STATUS;

/************************************************************************/
/* States in stage-state machine                                        */
/************************************************************************/
typedef enum{
	STAGE_INIT,
	STAGE_GET_TIME,
	STAGE_START,
	STAGE_WAIT,
	STAGE_DEINIT,
	STAGE_UPDATE_MRPP
} STAGE_STATES;


#endif /* MAINPG_H_ */