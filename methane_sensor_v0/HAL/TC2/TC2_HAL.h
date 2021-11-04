/*
 * TC2_HAL.h
 *
 * Created: 04-11-2021 19:00:14
 *  Author: Mainframe
 */ 


#ifndef TC2_HAL_H_
#define TC2_HAL_H_

#include <xc.h>
#include "../types.h"


#define MAX_CALLBACKS 5

void TC2_HAL_init();
void TC2_HAL_kill();
int8_t TC2_HAL_subscribe(callback_ptr cb);
void TC2_HAL_cancel(int8_t pointer);


#endif /* TC2_HAL_H_ */