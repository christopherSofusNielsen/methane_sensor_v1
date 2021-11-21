/*
 * main.c
 *
 * Created: 10/4/2021 4:27:30 PM
 *  Author: Mainframe
 */ 

#include <xc.h>
#include <util/delay.h>
#include "TEST/test_runner.h"
#include "PROGRAMS/MAINPG/MAINPG.h"
#include "PROGRAMS/CONFIG_MODE/CONFIG_MODE.h"
#include "HAL/PM/PM_HAL.h"
#include "HAL/UI/UI_HAL.h"

int main(void)
{
	//test_runner();
	
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
}