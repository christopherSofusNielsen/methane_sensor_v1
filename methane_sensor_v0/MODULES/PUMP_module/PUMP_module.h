/*
 * PUMP_module.h
 *
 * Created: 08-11-2021 16:36:08
 *  Author: Mainframe
 */ 


#ifndef PUMP_MODULE_H_
#define PUMP_MODULE_H_

#include <xc.h>
#include <stdbool.h>

void PUMP_start(uint16_t seconds);
bool PUMP_is_done();



#endif /* PUMP_MODULE_H_ */