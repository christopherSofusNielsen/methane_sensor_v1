/*
 * RN2483_HAL.c
 *
 * Created: 05-10-2021 15:49:01
 *  Author: Mainframe
 */ 

#include "RN2483_HAL.h"
#include "../../util/bit_operators.h"

void rn2483_init(){
	set_bit(RN2483_DDR, RN2483_PIN);
}

void rn2483_reset_active(){
	clear_bit(RN2483_PORT, RN2483_PIN);
}

void rn2483_reset_deactive(){
	set_bit(RN2483_PORT, RN2483_PIN);
}