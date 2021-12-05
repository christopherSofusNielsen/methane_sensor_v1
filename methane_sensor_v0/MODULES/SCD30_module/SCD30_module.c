/*
 * SCD30_module.c
 *
 * Created: 17-10-2021 14:53:00
 *  Author: Christopher S. Nielsen
 */ 

#include <util/delay.h>
#include <stdbool.h>
#include <stddef.h>
#include "SCD30_module.h"
#include "../../HAL/TWI/TWI_API.h"
#include "../../HAL/SCD30/SCD30_HAL.h"
#include "../../util/CRC8/CRC8.h"
#include "../../util/to_float.h"
#include "../../HAL/PM/PM_HAL.h"
#include "../../HAL/TC1/TC1_HAL.h"

/************************************************************************/
/* Constants I2C commands                                               */
/************************************************************************/
const uint8_t TRIGGER_CONT_MEASUREMENT[]={0x00, 0x10, 0x00, 0x00, 0x81};
const uint8_t SET_MEASUREMENT_INTERVAL[]={0x46, 0x00, 0x00, 0x02, 0xE3};
const uint8_t READ_DATA_CMD[]={0x03, 0x00};
const uint8_t DATA_READY_CMD[]={0x02, 0x02};

/************************************************************************/
/* Variables                                                            */
/************************************************************************/
static SCD30_PARAMETER co2_pars;
static SCD30_PARAMETER temp_pars;
static SCD30_PARAMETER hum_pars;
static uint16_t lowest_si;
static uint16_t highest_si;
static uint16_t si_counter;
	
/************************************************************************/
/* Local functions                                                      */
/************************************************************************/
static bool validate_data(uint8_t data[]);
static uint16_t bytes_2_uint(uint8_t data[], uint8_t scale);
static void vect_SCD_do_sample();
static SCD30_STATUS read_value(uint16_t *value);
static SCD30_STATUS init_measurement();
static void clear_sampling_data();

/************************************************************************/
/* Definitions                                                          */
/************************************************************************/

SCD30_STATUS SCD30_sensor_on(){
	SCD30_HAL_init();
	PM_HAL_SCD30_power(true);
	_delay_ms(2000);
	clear_sampling_data();
	return init_measurement();
}

void SCD30_sensor_off(){
	PM_HAL_SCD30_power(false);
}

void SCD30_init_c02_sampling(uint16_t samplingInterval, uint8_t nSamples, uint16_t data[]){
	co2_pars.cntSamples=0;
	co2_pars.nSamples=nSamples;
	co2_pars.samplingInterval=samplingInterval;
	co2_pars.data=data;
}

void SCD30_init_temp_sampling(uint16_t samplingInterval, uint8_t nSamples, uint16_t data[]){
	temp_pars.cntSamples=0;
	temp_pars.nSamples=nSamples;
	temp_pars.samplingInterval=samplingInterval;
	temp_pars.data=data;
}

void SCD30_init_humidity_sampling(uint16_t samplingInterval, uint8_t nSamples, uint16_t data[]){
	hum_pars.cntSamples=0;
	hum_pars.nSamples=nSamples;
	hum_pars.samplingInterval=samplingInterval;
	hum_pars.data=data;
}

bool SCD30_start_sampling(){
	//Calculate SIs
	bool valid=SCD30_calc_validate_SI(
	co2_pars.samplingInterval,
	temp_pars.samplingInterval,
	hum_pars.samplingInterval,
	&lowest_si,
	&highest_si
	);
	if(!valid) return false;
	
	//Wait for first sampling
	while(!SCD30_HAL_data_ready()){};
	
	//Set timer
	TC1_HAL_init(lowest_si, &vect_SCD_do_sample);
	
	//Take first sampling
	vect_SCD_do_sample();
	
	//Start timer
	TC1_HAL_start();
	return true;
}

void SCD30_deinit_sampling(){
	TC1_HAL_stop();
	clear_sampling_data();
}



bool SCD30_is_sampling_done(){
	return (
		co2_pars.nSamples==co2_pars.cntSamples && 
		temp_pars.nSamples==temp_pars.cntSamples && 
		hum_pars.nSamples==hum_pars.cntSamples); 
}

bool SCD30_calc_validate_SI(uint16_t co2_SI, uint16_t temp_SI, uint16_t hum_SI, uint16_t *lowest, uint16_t *highest){
	*highest=0;
	if(co2_SI>(*highest)){
		*highest=co2_SI;
	}
	if (temp_SI>(*highest)){
		*highest=temp_SI;
	}
	if (hum_SI>(*highest)){
		*highest=hum_SI;
	}
	*lowest=*highest;
	if(co2_SI!=0 && co2_SI<(*lowest)){
		*lowest=co2_SI;
	}
	if (temp_SI!=0 && temp_SI<(*lowest)){
		*lowest=temp_SI;
	}
	if(hum_SI!=0 && hum_SI<(*lowest)){
		*lowest=hum_SI;
	}
	
	//Check that all sampling intervals are dividable with lowest si
	if(co2_SI % *lowest != 0) return false;
	if(temp_SI % *lowest != 0) return false;
	if(hum_SI % *lowest != 0) return false;
	return true;
}

SCD30_STATUS SCD30_get_reading(uint16_t *co2, uint16_t *temp, uint16_t *humidity){
	while(!SCD30_HAL_data_ready()){};
	
	return read_all_values(co2, temp, humidity);
}

/************************************************************************/
/* Local functions                                                      */
/************************************************************************/
static void clear_sampling_data(){
	co2_pars.cntSamples=0;
	co2_pars.nSamples=0;
	co2_pars.samplingInterval=0;
	co2_pars.data=NULL;
	
	temp_pars.cntSamples=0;
	temp_pars.nSamples=0;
	temp_pars.samplingInterval=0;
	temp_pars.data=NULL;

	hum_pars.cntSamples=0;
	hum_pars.nSamples=0;
	hum_pars.samplingInterval=0;
	hum_pars.data=NULL;
	
	lowest_si=0;
	highest_si=0;
	si_counter=0;
}

static SCD30_STATUS init_measurement(){
	uint8_t status;
	status=TWI_API_write_data_stop(SCD30_ADDR,  SET_MEASUREMENT_INTERVAL, 5);
	if(status != TWI_CODE_SUCCESS) return SCD30_STATUS_FATAL_ERROR;
	
	status=TWI_API_write_data_stop(SCD30_ADDR,  TRIGGER_CONT_MEASUREMENT, 5);
	if(status != TWI_CODE_SUCCESS) return SCD30_STATUS_FATAL_ERROR;
	
	//Wait for data_ready pin to go high
	while(!SCD30_HAL_data_ready()){};
	
	return SCD30_STATUS_SUCCESS;
}

SCD30_STATUS read_all_values(uint16_t *co2, uint16_t *temp, uint16_t *humidity){
	uint8_t status;
	uint8_t data[18];
	*co2=0xffff;
	*temp=0xffff;
	*humidity=0xffff;
	
	//Check values are ready
	if(!SCD30_HAL_data_ready()){
		*co2=0xfffe;
		*temp=0xfffe;
		*humidity=0xfffe;
		return SCD30_STATUS_TRY_AGAIN;
	}
	
	//Set read pointer
	status=TWI_API_write_data_stop(SCD30_ADDR,   READ_DATA_CMD, 2);
	if(status != TWI_CODE_SUCCESS) return SCD30_STATUS_ERROR;
	
	_delay_ms(4);
	
	//Read values
	status=TWI_API_read_data_ack_end_nack_stop(SCD30_ADDR, data, 18);
	if(status != TWI_CODE_SUCCESS) return SCD30_STATUS_ERROR;
	
	//Validate co2
	if(!validate_data(data)) return SCD30_STATUS_ERROR;
	*co2=bytes_2_uint(data, 1);
	
	//Validate temp
	if(!validate_data(&data[6])) return SCD30_STATUS_ERROR;
	*temp=bytes_2_uint(&data[6], 10);
	
	//Validate humidity
	if(!validate_data(&data[12])) return SCD30_STATUS_ERROR;
	*humidity=bytes_2_uint(&data[12], 10);
	
	return SCD30_STATUS_SUCCESS;
}

static bool validate_data(uint8_t data[]){
	uint8_t crc=calc_crc8(data, 2);
	if(data[2]!=crc) return false;
	
	crc=calc_crc8(&data[3], 2);
	if(data[5]!=crc) return false;
	
	return true;
}

static uint16_t bytes_2_uint(uint8_t data[], uint8_t scale){
	
	to_float f;
	
	f.bytes[3]=data[0];
	f.bytes[2]=data[1];
	f.bytes[1]=data[3];
	f.bytes[0]=data[4];
	
	float value=(float) f.f *scale;
	if(value<0) return 0;
	return (uint16_t) value;
}

/************************************************************************/
/* Interrupts                                                           */
/************************************************************************/
static void vect_SCD_do_sample(){
	uint16_t co2, temp, humidity;
	
	read_all_values(&co2, &temp, &humidity);
	
	//Set values 
	if(co2_pars.cntSamples<co2_pars.nSamples &&  si_counter % co2_pars.samplingInterval==0){
		co2_pars.data[co2_pars.cntSamples++]=co2;
	}
	if(temp_pars.cntSamples<temp_pars.nSamples &&  si_counter % temp_pars.samplingInterval==0){
		temp_pars.data[temp_pars.cntSamples++]=temp;
	}
	if(hum_pars.cntSamples<hum_pars.nSamples &&  si_counter % hum_pars.samplingInterval==0){
		hum_pars.data[hum_pars.cntSamples++]=humidity;
	}
	
	//Wrap si_counter if at max
	si_counter+=lowest_si;
	if(si_counter==highest_si){
		si_counter=0;
	}
	
	//Stop sampling if all values are sampled
	if(
		co2_pars.cntSamples==co2_pars.nSamples && 
		temp_pars.cntSamples==temp_pars.nSamples &&
		hum_pars.cntSamples==hum_pars.nSamples
	){
		TC1_HAL_stop();			
	}
}

