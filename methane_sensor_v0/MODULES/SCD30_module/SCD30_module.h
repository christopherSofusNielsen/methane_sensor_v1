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
* Initializes the timer used when sampling.
* -> uint16_t samplingInterval: Sampling interval in seconds, must be grather than 2 s.
* -> uint16_t nSamples: Number of samples, must be grather than 2
* -> uint16_t data[]: Pointer to array where samples must be stored.                                                                     */
/************************************************************************/
void SCD30_init_sampling(uint16_t samplingInterval, uint16_t nSamples, uint16_t data[]);

/************************************************************************/
/* Stops the timer, hence the sampling.                                 */
/************************************************************************/
void SCD30_deinit_sampling();

/************************************************************************/
/* Start the sampling process, and takes the first sampling to time 0   */
/************************************************************************/
void SCD30_start_sampling();

/************************************************************************/
/* Returns true all samplings are taken.                                */
/************************************************************************/
bool SCD30_is_sampling_done();

/************************************************************************
* Takes one sample from the SCD30, the sensor must be power before calling this function.
* -> uint16_t *value: Pointer to where the sample must be stored.
* -> Returns SCD30_STATUS_SUCCESS when succesfull, 
	 otherwise SCD30_STATUS_TRY_AGAIN if no sample was ready in the internal buffer
	 or SCD30_STATUS_ERROR on error.
************************************************************************/
SCD30_STATUS SCD30_get_reading(uint16_t *value);



#endif /* SCD30_MODULE_H_ */