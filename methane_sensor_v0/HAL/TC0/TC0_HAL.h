/*
 * TC0_HAL.h
 *
 * Created: 01-11-2021 10:51:23
 *  Author: Christopher S. Nielsen
 */ 


#ifndef TC0_HAL_H_
#define TC0_HAL_H_
#include <xc.h>
#include <stdbool.h>

#include "../types.h"

void TC0_HAL_init(uint8_t match, callback_ptr cb);
void TC0_HAL_start();
void TC0_HAL_stop();





#endif /* TC0_HAL_H_ */