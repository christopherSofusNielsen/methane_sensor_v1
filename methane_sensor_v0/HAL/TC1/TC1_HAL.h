/*
 * TC1_HAL.h
 *
 * Created: 01-11-2021 15:49:28
 *  Author: Christopher S. Nielsen
 */ 


#ifndef TC1_HAL_H_
#define TC1_HAL_H_

#include <xc.h>
#include <stdbool.h>

#include "../types.h"

void TC1_HAL_init(uint16_t match, callback_ptr cb);
void TC1_HAL_start();
void TC1_HAL_stop();




#endif /* TC1_HAL_H_ */