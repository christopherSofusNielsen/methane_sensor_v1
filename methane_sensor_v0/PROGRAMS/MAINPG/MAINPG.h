/*
 * MAINPG.h
 *
 * Created: 05-11-2021 19:51:35
 *  Author: Mainframe
 */ 


#ifndef MAINPG_H_
#define MAINPG_H_

void MAINPG_start();


typedef enum{
	MAINPG_INIT_HW,
	
	MAINPG_LORA_JOIN_NETWORK,
	MAINPG_LORA_JOIN_TRY_AGAIN,
	MAINPG_LORA_JOIN_CONF_ERR,
	MAINPG_LORA_WAKEUP,
	
	MAINPG_INIT_RTC,
	MAINPG_SET_WAKEUP,
	MAINPG_CLEAR_WAKEUP,
	
	MAINPG_INIT_MRPP,
	MAINPG_STAGE_0,
	MAINPG_SEND_DATA,
	MAINPG_SEND_ALL_DATA,
	MAINPG_SEND_HEADER,
	MAINPG_SEND_TAIL,
	
	MAINPG_SETUP_SLEEP,
	MAINPG_SLEEP,
	
	MAINPG_FATAL_ERROR,
	MAINPG_END
} MAINPG_STATES;

typedef enum{
	STAGE_RUNNING,
	STAGE_DONE,
	STAGE_ERROR,
	STAGE_FATAL_ERROR
} STAGE_STATUS;

typedef enum{
	S0_INIT,
	S0_START,
	S0_WAIT,
	S0_DEINIT,
	S0_UPDATE_MRPP
} S0_STATES;

//typedef enum{
	//MAINPG_CODE_SUCCESS,
	//MAINPG_CODE_ERROR
//} MAINPG_ERR_CODE;


#endif /* MAINPG_H_ */