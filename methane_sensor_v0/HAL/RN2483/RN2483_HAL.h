/*
 * RN2483_HAL.h
 *
 * Created: 05-10-2021 15:48:27
 *  Author: Christopher S. Nielsen
 */ 


#ifndef RN2483_HAL_H_
#define RN2483_HAL_H_

#include <xc.h>

//Ports
#define RN2483_DDR DDRB
#define RN2483_PORT PORTB
#define RN2483_PIN 4

void rn2483_init();
void rn2483_reset_active();
void rn2483_reset_deactive();





#endif /* RN2483_HAL_H_ */