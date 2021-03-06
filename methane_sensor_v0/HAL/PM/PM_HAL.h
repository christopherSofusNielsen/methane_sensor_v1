/*
 * PM_HAL.h
 *
 * Created: 18-10-2021 09:59:19
 *  Author: Christopher S. Nielsen
 */ 


#ifndef PM_HAL_H_
#define PM_HAL_H_
#include <stdbool.h>

void PM_HAL_enter_power_down();

//ADC
void PM_HAL_adc_power_init();
void PM_HAL_adc_power(bool on);

//Methane Sensor
void PM_HAL_meth_power_init();
void PM_HAL_meth_power(bool on);

//SCD30
void PM_HAL_SCD30_power_init();
void PM_HAL_SCD30_power(bool on);

//Boost Converter BC
void PM_HAL_BC_power_init();
void PM_HAL_BC_power(bool on);

//LED
void PM_HAL_LED_power_init();
void PM_HAL_LED_power(bool on);
void PM_HAL_LED_toggle();




#endif /* PM_HAL_H_ */