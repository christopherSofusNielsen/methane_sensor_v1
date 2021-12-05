/*
 * SCD30_module.h
 *
 * Created: 17-10-2021 14:53:10
 *  Author: Christopher S. Nielsen
 */ 


#ifndef SCD30_MODULE_H_
#define SCD30_MODULE_H_

//The I2C slave address of the SCD30 without the W/R bit
#define SCD30_ADDR 0x61

#include <xc.h>
#include <stdbool.h>

/************************************************************************/
/* Status Type                                                          */
/************************************************************************/
typedef enum {
	SCD30_STATUS_SUCCESS,
	SCD30_STATUS_ERROR,
	SCD30_STATUS_FATAL_ERROR,
	SCD30_STATUS_TRY_AGAIN
}SCD30_STATUS;

/************************************************************************/
/* Parameter struct used internally                                     */
/************************************************************************/
typedef struct __SCD30_PARAMETER{
	uint8_t nSamples;
	uint8_t cntSamples;
	uint16_t *data;
	uint16_t samplingInterval;
}SCD30_PARAMETER;


/************************************************************************
* Turns on the SCD30, initialize the hardware and sensor,
* waits for the internal buffer to be filled with the first sampling.
* Takes up to 2s, and should be called after each device power up.
* -> Returns SCD30_STATUS_SUCCESS when succesfull, otherwise SCD30_STATUS_FATAL_ERROR
************************************************************************/
SCD30_STATUS SCD30_sensor_on();

/************************************************************************/
/* Turns of the SCD30                                                   */
/************************************************************************/
void SCD30_sensor_off();

/************************************************************************
* Initializes co2 sampling.
* Don't call if co2 should not be sampled.
* -> uint16_t samplingInterval: Sampling interval in seconds, must be grather than 2 s.
* -> uint16_t nSamples: Number of samples, must be grather than 2
* -> uint16_t data[]: Pointer to array where samples must be stored.                                                                     */
/************************************************************************/
void SCD30_init_c02_sampling(uint16_t samplingInterval, uint8_t nSamples, uint16_t data[]);

/************************************************************************
* Initializes temp sampling.
* Don't call if co2 should not be sampled.
* -> uint16_t samplingInterval: Sampling interval in seconds, must be grather than 2 s.
* -> uint16_t nSamples: Number of samples, must be grather than 2
* -> uint16_t data[]: Pointer to array where samples must be stored.                                                                     */
/************************************************************************/
void SCD30_init_temp_sampling(uint16_t samplingInterval, uint8_t nSamples, uint16_t data[]);

/************************************************************************
* Initializes humidity sampling.
* Don't call if co2 should not be sampled.
* -> uint16_t samplingInterval: Sampling interval in seconds, must be grather than 2 s.
* -> uint16_t nSamples: Number of samples, must be grather than 2
* -> uint16_t data[]: Pointer to array where samples must be stored.                                                                     */
/************************************************************************/
void SCD30_init_humidity_sampling(uint16_t samplingInterval, uint8_t nSamples, uint16_t data[]);

/************************************************************************
* Start the sampling process, and takes the first sampling to time 0
* -> Returns true if co2, temp and humidity is configured correct.
*	 The sampling intervals must either be 0 (no sampling), 
*	 or be dividable with the lowest sampling interval
************************************************************************/
bool SCD30_start_sampling();

/************************************************************************/
/* Stops the timer, hence the sampling. Clears configuration.           */
/************************************************************************/
void SCD30_deinit_sampling();

/************************************************************************/
/* Returns true all samplings are taken.                                */
/************************************************************************/
bool SCD30_is_sampling_done();

/************************************************************************
* Returns true if sampling intervals are valid.
* All sampling intervals must be dividable with the lowest sampling interval.
* -> uint16_t co2_SI: CO2 sampling interval, use 0 if not sampled.
* -> uint16_t temp_SI: Temperature sampling interval, use 0 if not sampled.
* -> uint16_t hum_SI: Humidity sampling interval, use 0 if not sampled.
* -> uint16_t *lowest: Pointer to where the lowest sampling interval can be stored.
* -> uint16_t *highest: Pointer to where the highest sampling interval can be stored.
************************************************************************/
bool SCD30_calc_validate_SI(uint16_t co2_SI, uint16_t temp_SI, uint16_t hum_SI, uint16_t *lowest, uint16_t *highest);

/************************************************************************
* Takes one sample from the SCD30, the sensor must be powered before calling this function.
* -> uint16_t *co2: Pointer to where the CO2 sample must be stored.
* -> uint16_t *temp: Pointer to where the temperature sample must be stored.
* -> uint16_t *humidity: Pointer to where the humidity sample must be stored.
* -> Returns SCD30_STATUS_SUCCESS when succesfull, 
	 otherwise SCD30_STATUS_TRY_AGAIN if no sample was ready in the internal buffer
	 or SCD30_STATUS_ERROR on error.
************************************************************************/
SCD30_STATUS SCD30_get_reading(uint16_t *co2, uint16_t *temp, uint16_t *humidity);

/************************************************************************/
/* Read all values, only for test                                       */
/************************************************************************/
SCD30_STATUS read_all_values(uint16_t *co2, uint16_t *temp, uint16_t *humidity);


#endif /* SCD30_MODULE_H_ */