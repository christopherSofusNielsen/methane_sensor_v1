/*
 * PUMP_module.h
 *
 * Created: 08-11-2021 16:36:08
 *  Author: Christopher S. Nielsen
 */ 


#ifndef PUMP_MODULE_H_
#define PUMP_MODULE_H_

#include <xc.h>
#include <stdbool.h>

/************************************************************************
* Turns on the air pump for x seconds
* -> uint16_t seconds: The number of seconds the pump should be running.
************************************************************************/
void PUMP_start(uint16_t seconds);

/************************************************************************/
/* Returns true when the air pump is done/not running                   */
/************************************************************************/
bool PUMP_is_done();



#endif /* PUMP_MODULE_H_ */