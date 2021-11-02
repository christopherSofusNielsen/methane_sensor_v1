/*
 * SCD30_HAL.c
 *
 * Created: 17-10-2021 15:11:56
 *  Author: Mainframe
 */ 

#include "SCD30_HAL.h"
#include <xc.h>
#include "../../util/bit_operators.h"
#include <stdbool.h>

void SCD30_HAL_init(){
	clear_bit(DDRC, 6);
	clear_bit(PORTC, 6);
}

bool SCD30_HAL_data_ready(){
	return get_bit(PINC, 6);
}