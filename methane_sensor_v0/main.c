/*
 * main.c
 *
 * Created: 10/4/2021 4:27:30 PM
 *  Author: Christopher S. Nielsen
 */ 

#include <xc.h>
#include <util/delay.h>
#include "TEST/test_runner.h"
#include "PROGRAMS/MAINPG/MAINPG.h"
#include "PROGRAMS/CONFIG_MODE/CONFIG_MODE.h"
#include "HAL/PM/PM_HAL.h"
#include "HAL/UI/UI_HAL.h"

//#define _RUN_TEST

int main(void)
{
	#ifdef _RUN_TEST
		test_runner();
	#endif // _RUN_TEST
	
	#ifndef _RUN_TEST 

	PM_HAL_LED_power_init();
	UI_HAL_jumper_init();
	
	while(1)
	{
		
		if(UI_HAL_is_config_mode()){
			PM_HAL_LED_power(true);
			CONF_enter();
			PM_HAL_LED_power(false);
		}else{
			MAINPG_start();
		}
		_delay_ms(5000);
	}
	#endif
}