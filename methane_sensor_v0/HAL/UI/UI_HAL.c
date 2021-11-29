/*
 * UI_HAL.c
 *
 * Created: 16-11-2021 12:18:27
 *  Author: Christopher S. Nielsen
 */ 

#include <xc.h>

#include "UI_HAL.h"
#include "../../util/bit_operators.h"

void UI_HAL_jumper_init(){
	clear_bit(DDRA, 6);
	set_bit(PORTA, 6); //Pull up, default config mode
}

bool UI_HAL_is_config_mode(){
	return get_bit(PINA, 6);
}

