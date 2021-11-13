/*
 * ADC_module.h
 *
 * Created: 18-10-2021 17:20:06
 *  Author: Mainframe
 */ 


#ifndef ADC_MODULE_H_
#define ADC_MODULE_H_

#define ADC_SLAVE_ADDR 0x4D

#include <stdbool.h>

typedef enum {
	ADC_STATUS_SUCCESS,
	ADC_STATUS_ERROR,
	ADC_STATUS_FATAL_ERROR,
	ADC_STATUS_TRY_AGAIN
}ADC_STATUS;


ADC_STATUS ADC_init_sampling(uint8_t samplingInterval, uint16_t nSamples, uint16_t data[]);
void ADC_deinit_sampling();
void ADC_start_sampling();
bool ADC_is_sampling_done();
ADC_STATUS ADC_get_value(uint16_t *value);
void ADC_meth_sens_power_on(uint8_t powerUpTime);
void ADC_meth_sens_power_off();
bool ADC_meth_sens_ready();
void ADC_set_conf_parameters(float Vcc, float Rrl, float PPMfactor);












#endif /* ADC_MODULE_H_ */