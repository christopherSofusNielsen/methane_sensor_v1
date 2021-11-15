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

int main(void)
{
	//test_runner();
	//return 0;
	while(1)
	{
		if(1){
			CONF_enter();
		}else{
			MAINPG_start();
		}
		_delay_ms(5000);
		
	}
}