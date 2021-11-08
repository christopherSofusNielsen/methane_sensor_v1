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

int main(void)
{
	test_runner();
	
	while(1)
    {
		//MAINPG_start();
		_delay_ms(3000);
        
    }
	
}