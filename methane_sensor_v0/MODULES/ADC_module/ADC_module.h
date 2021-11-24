/*
 * ADC_module.h
 *
 * Created: 18-10-2021 17:20:06
 *  Author: Christopher S. Nielsen
 */ 


#ifndef ADC_MODULE_H_
#define ADC_MODULE_H_



//The I2C slave address of the ADC without the W/R bit
#define ADC_SLAVE_ADDR 0x4D

//The heat up interval for the methane sensor
#define ADC_HEAT_TIME_INTERVAL 60

/************************************************************************/
/* Dependencies                                                        */
/************************************************************************/
#include <stdbool.h>


/************************************************************************/
/* Status Type                                                          */
/************************************************************************/
typedef enum {
	ADC_STATUS_SUCCESS,
	ADC_STATUS_ERROR,
	ADC_STATUS_FATAL_ERROR,
	ADC_STATUS_TRY_AGAIN
}ADC_STATUS;



/************************************************************************
* Initialize timer, power on ADC and test that sensor is responding.
* -> uint8_t samplingInterval: Sampling interval in second, must be grather than 2 s.
* -> uint16_t nSamples: Number of samples, must be grather than 2
* -> uint16_t data[]: Pointer to array where samples must be stored.
* -> Returns ADC_STATUS_SUCCESS when succesfull, otherwise ADC_STATUS_FATAL_ERROR
/************************************************************************/
ADC_STATUS ADC_init_sampling(uint8_t samplingInterval, uint16_t nSamples, uint16_t data[]);

/************************************************************************/
/* Stops the timer and turns off power to the ADC                       */
/************************************************************************/
void ADC_deinit_sampling();

/************************************************************************
* Start the timer, hence the sampling. 
* Takes the first measurement to time 0
/************************************************************************/
void ADC_start_sampling();

/************************************************************************/
/* Returns true when all samplings are taken.                           */
/************************************************************************/
bool ADC_is_sampling_done();

/************************************************************************
* Takes one sample from the ADC.
* -> uint16_t *value: Pointer to where the sample must be stored.
* -> Returns ADC_STATUS_SUCCESS when succesfull, otherwise ADC_STATUS_ERROR 
/************************************************************************/
ADC_STATUS ADC_get_value(uint16_t *value);

/************************************************************************
* Turns on the methane sensor.
* -> uint8_t powerUpTime: The time in minutes the sensor should heat up, 
*						  before it's ready.
/************************************************************************/
void ADC_meth_sens_power_on(uint8_t powerUpTime);

/************************************************************************/
/* Turns of the methane sensor.                                         */
/************************************************************************/
void ADC_meth_sens_power_off();

/************************************************************************/
/* Return true when the methane is heated and ready to be used.			*/
/************************************************************************/
bool ADC_meth_sens_ready();

/************************************************************************
* Sets the constant values used to convert from voltage to PPM of methane.
* This function should be called at least after power up the device.
* -> float Vcc: The supplied voltage for the ADC.
* -> float Rrl: The value of the Rsense resistor.
* -> float PPMfactor: The factor multiplied to the voltage to get the PPM of methane.
/************************************************************************/
void ADC_set_conf_parameters(float Vcc, float Rrl, float PPMfactor);












#endif /* ADC_MODULE_H_ */