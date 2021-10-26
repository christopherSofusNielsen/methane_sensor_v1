/*
 * ADC_module.h
 *
 * Created: 18-10-2021 17:20:06
 *  Author: Mainframe
 */ 


#ifndef ADC_MODULE_H_
#define ADC_MODULE_H_

#define ADC_SLAVE_ADDR 0x4D

typedef enum {
	ADC_STATUS_SUCCESS,
	ADC_STATUS_ERROR,
	ADC_STATUS_FATAL_ERROR,
	ADC_STATUS_TRY_AGAIN
}ADC_STATUS;


ADC_STATUS ADC_init(uint8_t interval);
ADC_STATUS ADC_get_reading(uint16_t *value);
uint16_t calc_ppm_methane(uint16_t bit_value);





#endif /* ADC_MODULE_H_ */