/*
 * main.c
 *
 * Created: 10/4/2021 4:27:30 PM
 *  Author: Mainframe
 */ 


#define F_CPU 8000000UL	

#include <xc.h>
#include "TEST/lora-module/test_lora_module.h"
#include "TEST/SCD30_module/test_SCD30_module.h"
#include "TEST/PM_HAL/test_PM_HAL.h"
#include "TEST/EEPROM_module/test_EEPROM_module.h"
#include "TEST/ADC_module/test_ADC_module.h"
#include "TEST/RTC_module/test_RTC_module.h"
#include "TEST/TC0_HAL/test_TC0_HAL.h"
#include "TEST/TC1_HAL/test_TC1_HAL.h"

int main(void)
{
	//test_lora_module_start();
	test_SCD30_module_start();
    //test_PM_HAL_start();
	//test_EEPROM_module_start();
	//test_ADC_module_start();
	//test_RTC_module_start();
	//test_TC0_HAL_start();
	//test_TC1_HAL_start();
	
	while(1)
    {
        //TODO:: Please write your application code 
    }
	
}