/*
 * MAINPG.c
 *
 * Created: 05-11-2021 19:51:28
 *  Author: Mainframe
 */ 
#define print_debug(txt) send_msg(txt)

#include <util/delay.h>
#include <stdbool.h>
#include <xc.h>

#include "../../util/bit_operators.h"

#include "MAINPG.h"

//HAL
#include "../../HAL/UART1/UART1_HAL.h"
#include "../../HAL/UART0/UART0_HAL.h"
#include "../../HAL/TWI/TWI_HAL.h"
#include "../../HAL/TC2/TC2_HAL.h"
#include "../../HAL/PM/PM_HAL.h"
#include "../../HAL/RN2483/RN2483_HAL.h"
#include "../../HAL/SCD30/SCD30_HAL.h"

//Module
#include "../../MODULES/RTC_module/RTC_module.h"
#include "../../MODULES/MRPP_module/mrpp.h"
#include "../../MODULES/MRPP_module/types.h"
#include "../../MODULES/ADC_module/ADC_module.h"
#include "../../MODULES/SCD30_module/SCD30_module.h"
#include "../../MODULES/LORA_module/lora_module.h"
#include "../../MODULES/PUMP_module/PUMP_module.h"
#include "../../MODULES/LED_module/LED_module.h"

//Functions
static RTC_STATUS set_wakeup();
static LM_STATUS join_lora();
static MAINPG_STATES decode_join_response(LM_STATUS status);
static MAINPG_STATES decode_header_tail_response(LM_STATUS status, MAINPG_STATES success, MAINPG_STATES tryAgain);
static MAINPG_STATES decode_body_response(int16_t bodyIndex, LM_STATUS status, MAINPG_STATES success);
static LM_STATUS send_body(int16_t bodyIndex);
static LM_STATUS send_header();
static LM_STATUS send_tail();
static MAINPG_STATES decode_stage_response(STAGE_STATUS status, MAINPG_STATES onDone);
static STAGE_STATUS stage_0();
static STAGE_STATUS stage_1();
static STAGE_STATUS stage_2();
static STAGE_STATUS stage_3();


//Global variabels
static STAGE_STATES state_s0=STAGE_INIT;
static STAGE_STATES state_s1=STAGE_INIT;
static STAGE_STATES state_s2=STAGE_INIT;
static STAGE_STATES state_s3=STAGE_INIT;

static int16_t bodyIndex=-1;
static uint16_t co2_data[MAX_CO2_SAMLPES];
static uint16_t meth_data[MAX_METH_SAMLPES];
static uint8_t ts[4];
static Datetime dt;
static COLLECTION cols[]={
	{.samplingInterval=2, .samplings=2, .type=T_INT16},
	{.samplingInterval=2, .samplings=2, .type=T_INT16},
	{.samplingInterval=2, .samplings=2, .type=T_INT16},
	{.samplingInterval=2, .samplings=2, .type=T_INT16},
	{.samplingInterval=2, .samplings=2, .type=T_INT16},
	{.samplingInterval=2, .samplings=2, .type=T_INT16},
	{.samplingInterval=2, .samplings=2, .type=T_INT16},
};
static uint8_t colsNumber=7;

static void send_msg(const char msg[]);


void MAINPG_start(){
	MAINPG_STATES state=MAINPG_INIT_HW;
	MAINPG_STATES comeBackToState;
	RTC_STATUS rtcStatus;
	LM_STATUS lmStatus;
	STAGE_STATUS stageStatus;
	bool fromSleep=false;
	//bool fromSleep=true;
	
	
	while(1){
		switch(state){
			case MAINPG_INIT_HW:
				
				uart0_hal_init();
				uart1_hal_init();
				TWI_HAL_init();
				TC2_HAL_init();
				rn2483_init();
				SCD30_HAL_init();
				PM_HAL_adc_power_init();
				PM_HAL_meth_power_init();
				PM_HAL_BC_power_init();
				PM_HAL_SCD30_power_init();
				PM_HAL_LED_power_init();
				
				//Set pull up
				set_bit(PORTB, 0);
				set_bit(PORTB, 1);
				
				state=fromSleep?MAINPG_LORA_WAKEUP:MAINPG_LORA_JOIN_NETWORK;
			break;
			/************************************************************************/
			/* LORA                                                                 */
			/************************************************************************/
			case MAINPG_LORA_JOIN_NETWORK:
				print_debug("Join\n\r");
				LED_start_try_join();
				lmStatus=join_lora();
				LED_stop_try_join();
				state=decode_join_response(lmStatus);
			break;
			
			case MAINPG_LORA_JOIN_SUCCESS:
				LED_join_success();
				state=MAINPG_INIT_RTC;
			break;
			
			case MAINPG_LORA_WAKEUP:
				print_debug("LoRa WAKE UP\n\r");
				lmStatus=LM_wake_up();
				state=lmStatus==LM_STATUS_SUCCESS?MAINPG_INIT_RTC:MAINPG_FATAL_ERROR;
			break;
			
			case MAINPG_LORA_JOIN_TRY_AGAIN:
				print_debug("Try again\n\r");
				LED_join_denied();
				_delay_ms(5000);
				state=MAINPG_LORA_JOIN_NETWORK;
			break;
			
			case MAINPG_LORA_JOIN_CONF_ERR:
				LED_join_conf_err();
				print_debug("Conf err\n\r");
				state=MAINPG_END;
			break;
			
			
			/************************************************************************/
			/* RTC                                                                  */
			/************************************************************************/
			case MAINPG_INIT_RTC:
				print_debug("INIT RTC\n\r");
				rtcStatus=RTC_set_clock_out(1);
				state=rtcStatus==RTC_STATUS_SUCCESS?MAINPG_CLEAR_WAKEUP:MAINPG_FATAL_ERROR;
				//state=rtcStatus==RTC_STATUS_SUCCESS?MAINPG_INIT_MRPP:MAINPG_FATAL_ERROR;
			break;
			
			case MAINPG_CLEAR_WAKEUP:
				print_debug("Clear interrupt\n\r");
				rtcStatus=RTC_clear_wake_up_interrupt();
				state=rtcStatus==RTC_STATUS_SUCCESS?MAINPG_SET_WAKEUP:MAINPG_FATAL_ERROR;
			break;
			
			case MAINPG_SET_WAKEUP:
				print_debug("Set wake interrupt\n\r");
				rtcStatus=set_wakeup();
				state=rtcStatus==RTC_STATUS_SUCCESS?MAINPG_INIT_MRPP:MAINPG_FATAL_ERROR;
			break;
			
			/************************************************************************/
			/* MRPP and data sampling                                               */
			/************************************************************************/
			case MAINPG_INIT_MRPP:
				print_debug("Mrpp init\n\r");
				MRPP_init_group(cols, colsNumber);
				state=MAINPG_SEND_HEADER;
			break;
			
			case MAINPG_SEND_HEADER:
				if(!LM_is_free()){
					state=MAINPG_SEND_HEADER;
					break;
				}
				lmStatus=send_header();
				state=decode_header_tail_response(lmStatus, MAINPG_STAGE_0, MAINPG_SEND_HEADER);
			break;
			
			case MAINPG_STAGE_0:
				comeBackToState=MAINPG_STAGE_0;
				stageStatus=stage_0();
				state=decode_stage_response(stageStatus, MAINPG_STAGE_1);
			break;
			
			case MAINPG_STAGE_1:
				comeBackToState=MAINPG_STAGE_1;
				stageStatus=stage_1();
				state=decode_stage_response(stageStatus, MAINPG_STAGE_2);
			break;
			
			case MAINPG_STAGE_2:
				comeBackToState=MAINPG_STAGE_2;
				stageStatus=stage_2();
				state=decode_stage_response(stageStatus, MAINPG_STAGE_3);
			break;
			
			case MAINPG_STAGE_3:
				comeBackToState=MAINPG_STAGE_3;
				stageStatus=stage_3();
				state=decode_stage_response(stageStatus, MAINPG_SEND_ALL_DATA);
			break;
			
			case MAINPG_SEND_DATA:
				state=comeBackToState;
				if(MRPP_is_body_package_ready(&bodyIndex) && LM_is_free()){
					lmStatus=send_body(bodyIndex);
					state=decode_body_response(bodyIndex, lmStatus, comeBackToState);
				}
			break;
			
			case MAINPG_SEND_ALL_DATA:
				if(MRPP_all_body_package_sent()){
					state=MAINPG_SEND_TAIL;
				}else{
					comeBackToState=MAINPG_SEND_ALL_DATA;
					state=MAINPG_SEND_DATA;
				}
			break;
			
			case MAINPG_SEND_TAIL:	
				if(!LM_is_free()){
					state=MAINPG_SEND_TAIL;
					break;
				}
				lmStatus=send_tail();
				state=decode_header_tail_response(lmStatus, MAINPG_SETUP_SLEEP, MAINPG_SEND_TAIL);
				
			break;
			
			/************************************************************************/
			/* Go to sleep                                                          */
			/************************************************************************/
			case MAINPG_SETUP_SLEEP:
				lmStatus=LM_put_to_sleep();
				// Set to false, so when it starts up it will do a restart of LORA module
				fromSleep=lmStatus==LM_STATUS_SUCCESS;
				//Make sure all peripherals are turned of
				PM_HAL_adc_power(false);
				PM_HAL_SCD30_power(false);
				PM_HAL_BC_power(false);
				PM_HAL_meth_power(false);
				PM_HAL_LED_power(false);
				
				state=MAINPG_SLEEP;
			break;
			
			case MAINPG_SLEEP:
				print_debug("Sleep\n\r");
				PM_HAL_enter_power_down();
				print_debug("Awake\n\r");
				state=MAINPG_INIT_HW;
			break;
			
			
			/************************************************************************/
			/* Error handling                                                       */
			/************************************************************************/
			case MAINPG_FATAL_ERROR:
				print_debug("Fatal error\n\r");
				LED_fatal_err();
				state=MAINPG_END;
			break;
			
			case MAINPG_END:
				print_debug("END\n\r");
				return;
			break;	
		}
	}
}


/************************************************************************/
/* Stage 0                                                              */
/************************************************************************/
static STAGE_STATUS stage_0(){
	ADC_STATUS adcStatus;
	SCD30_STATUS scd30Status;
	while(1){
		switch(state_s0){
			case STAGE_INIT:
				scd30Status=SCD30_init_sampling(cols[0].samplingInterval, cols[0].samplings, co2_data);
				if(scd30Status!=SCD30_STATUS_SUCCESS) return STAGE_FATAL_ERROR;
			
				state_s0=STAGE_GET_TIME;
			break;
			
			case STAGE_GET_TIME:
				RTC_get_current_time(&dt);
				state_s0=STAGE_START;
			break;
			
			case STAGE_START:
				adcStatus=ADC_meth_sens_power_on(1); //Should be changed
				if(adcStatus!=ADC_STATUS_SUCCESS) return STAGE_FATAL_ERROR;
			
				SCD30_start_sampling();
			
				state_s0=STAGE_WAIT;
			break;
			
			case STAGE_WAIT:
				if(!ADC_meth_sens_ready() || !SCD30_is_sampling_done()) return STAGE_RUNNING;
				state_s0=STAGE_UPDATE_MRPP;
			break;
			
			case STAGE_UPDATE_MRPP:
				RTC_datetime_to_ts(dt, ts);
				MRPP_add_collection_data_INT16(1, ts, co2_data);
				state_s0=STAGE_DEINIT;
			break;
			
			case STAGE_DEINIT:
				SCD30_deinit_sampling();
				return STAGE_DONE;
			break;
		}
	}
}

/************************************************************************/
/* Stage 1                                                              */
/************************************************************************/
static STAGE_STATUS stage_1(){
	ADC_STATUS adcStatus;
	SCD30_STATUS scd30Status;
	while(1){
		switch(state_s1){
			case STAGE_INIT:
				scd30Status=SCD30_init_sampling(cols[1].samplingInterval, cols[1].samplings, co2_data);
				if(scd30Status!=SCD30_STATUS_SUCCESS) return STAGE_FATAL_ERROR;
				
				adcStatus=ADC_init_sampling(cols[2].samplingInterval, cols[2].samplings, meth_data);
				if(adcStatus!=ADC_STATUS_SUCCESS) return STAGE_FATAL_ERROR;
				
				state_s1=STAGE_GET_TIME;
			break;
			
			case STAGE_GET_TIME:
				RTC_get_current_time(&dt);
				
				state_s1=STAGE_START;
			break;
			
			case STAGE_START:
				SCD30_start_sampling();
				ADC_start_sampling();
				
				state_s1=STAGE_WAIT;
			break;
			
			case STAGE_WAIT:
				if(!SCD30_is_sampling_done() || !ADC_is_sampling_done()) return STAGE_RUNNING;
				state_s1=STAGE_UPDATE_MRPP;
			break;
			
			case STAGE_UPDATE_MRPP:
				RTC_datetime_to_ts(dt, ts);
				MRPP_add_collection_data_INT16(2, ts, co2_data);
				MRPP_add_collection_data_INT16(3, ts, meth_data);
				state_s1=STAGE_DEINIT;
			break;
			
			case STAGE_DEINIT:
				SCD30_deinit_sampling();
				ADC_deinit_sampling();
				return STAGE_DONE;
			break;
			
		}
	}
}

/************************************************************************/
/* Stage 2                                                              */
/************************************************************************/
static STAGE_STATUS stage_2(){
	ADC_STATUS adcStatus;
	SCD30_STATUS scd30Status;
	uint16_t seconds=10;
	while(1){
		switch(state_s2){
			case STAGE_INIT:
				scd30Status=SCD30_init_sampling(cols[3].samplingInterval, cols[3].samplings, co2_data);
				if(scd30Status!=SCD30_STATUS_SUCCESS) return STAGE_FATAL_ERROR;
			
				adcStatus=ADC_init_sampling(cols[4].samplingInterval, cols[4].samplings, meth_data);
				if(adcStatus!=ADC_STATUS_SUCCESS) return STAGE_FATAL_ERROR;
			
				state_s2=STAGE_GET_TIME;
			break;
			
			case STAGE_GET_TIME:
				RTC_get_current_time(&dt);
			
				state_s2=STAGE_START;
			break;
			
			case STAGE_START:
				SCD30_start_sampling();
				ADC_start_sampling();
				PUMP_start(seconds);
			
				state_s2=STAGE_WAIT;
			break;
			
			case STAGE_WAIT:
				if(!SCD30_is_sampling_done() || !ADC_is_sampling_done() || !PUMP_is_done()) return STAGE_RUNNING;
				state_s2=STAGE_UPDATE_MRPP;
			break;
			
			case STAGE_UPDATE_MRPP:
				RTC_datetime_to_ts(dt, ts);
				MRPP_add_collection_data_INT16(4, ts, co2_data);
				MRPP_add_collection_data_INT16(5, ts, meth_data);
				state_s2=STAGE_DEINIT;
			break;
			
			case STAGE_DEINIT:
				SCD30_deinit_sampling();
				ADC_deinit_sampling();
				return STAGE_DONE;
			break;
			
		}
	}
}


/************************************************************************/
/* Stage 3                                                              */
/************************************************************************/
static STAGE_STATUS stage_3(){
	ADC_STATUS adcStatus;
	SCD30_STATUS scd30Status;
	while(1){
		switch(state_s3){
			case STAGE_INIT:
				scd30Status=SCD30_init_sampling(cols[5].samplingInterval, cols[5].samplings, co2_data);
				if(scd30Status!=SCD30_STATUS_SUCCESS) return STAGE_FATAL_ERROR;
				
				adcStatus=ADC_init_sampling(cols[6].samplingInterval, cols[6].samplings, meth_data);
				if(adcStatus!=ADC_STATUS_SUCCESS) return STAGE_FATAL_ERROR;
			
				state_s3=STAGE_GET_TIME;
			break;
			
			case STAGE_GET_TIME:
				RTC_get_current_time(&dt);
				
				state_s3=STAGE_START;
			break;
			
			case STAGE_START:
				SCD30_start_sampling();
				ADC_start_sampling();
				
				state_s3=STAGE_WAIT;
			break;
			
			case STAGE_WAIT:
				if(!SCD30_is_sampling_done() || !ADC_is_sampling_done()) return STAGE_RUNNING;
				state_s3=STAGE_UPDATE_MRPP;
			break;
			
			case STAGE_UPDATE_MRPP:
				RTC_datetime_to_ts(dt, ts);
				MRPP_add_collection_data_INT16(6, ts, co2_data);
				MRPP_add_collection_data_INT16(7, ts, meth_data);
				state_s3=STAGE_DEINIT;
			break;
			
			case STAGE_DEINIT:
				SCD30_deinit_sampling();
				ADC_deinit_sampling();
				return STAGE_DONE;
			break;
		}
	}
}

/************************************************************************/
/* Helper functions                                                     */
/************************************************************************/
static MAINPG_STATES decode_stage_response(STAGE_STATUS status, MAINPG_STATES onDone){
	switch(status){
		case STAGE_RUNNING:
			return MAINPG_SEND_DATA;
		
		case STAGE_DONE:
			return onDone;
			
		default:
			return MAINPG_FATAL_ERROR;
	}
}

static RTC_STATUS set_wakeup(){
	//uint8_t samplingProcessInterval=1;
	//return RTC_set_wake_up_interrupt(samplingProcessInterval);
	return RTC_set_wake_up_interrupt_minutes(6);
}

static LM_STATUS join_lora(){
	char deveui[]="0004A30B00F4547A";
	char appeui[]="70B3D57ED003F844";
	char appkey[]="B88AD6D25A3B27C69A01F74C53F9A179";
	
	return LM_join_network(deveui, appeui, appkey);
}

static MAINPG_STATES decode_join_response(LM_STATUS status){
	switch(status){
		case LM_STATUS_SUCCESS:
			return MAINPG_LORA_JOIN_SUCCESS;
		case LM_STATUS_TRY_AGAIN:
			return MAINPG_LORA_JOIN_TRY_AGAIN;
		case LM_STATUS_CONF_ERR:
			return MAINPG_LORA_JOIN_CONF_ERR;
		default:
			return MAINPG_FATAL_ERROR;		
	}	
}

static LM_STATUS send_body(int16_t bodyIndex){
	uint8_t package[51];
	uint8_t package_length;
	
	MRPP_get_ready_body_package(bodyIndex, package, &package_length);
	return LM_send_uplink(package, package_length);
}

static MAINPG_STATES decode_body_response(int16_t bodyIndex, LM_STATUS status, MAINPG_STATES success){
	switch(status){
		case LM_STATUS_SUCCESS:
		case LM_STATUS_MAC_ERR:
		case LM_STATUS_INV_DATA_LEN:
			MRPP_set_body_sent(bodyIndex);
			return success;
		
		case LM_STATUS_TRY_AGAIN:
			return success;
		
		default:
			return MAINPG_FATAL_ERROR;
	}
}

static LM_STATUS send_header(){
	uint8_t package[51];
	uint8_t package_length;
	
	MRPP_get_header_package(package, &package_length);
	return LM_send_uplink(package, package_length);
}

static LM_STATUS send_tail(){
	uint8_t package[51];
	uint8_t package_length;
	
	MRPP_get_tail_package(package, &package_length);
	return LM_send_uplink(package, package_length);
}

static MAINPG_STATES decode_header_tail_response(LM_STATUS status, MAINPG_STATES success, MAINPG_STATES tryAgain){
	switch(status){
		case LM_STATUS_SUCCESS:
		case LM_STATUS_MAC_ERR:
		case LM_STATUS_INV_DATA_LEN:
			return success;
			
		case LM_STATUS_TRY_AGAIN:
			return tryAgain;
			
		default:
			return MAINPG_FATAL_ERROR;
	}
}

/************************************************************************/
/* Test functions                                                       */
/************************************************************************/
static void send_msg(const char msg[]){
	uart0_hal_send_string(msg);
	_delay_ms(100);
}