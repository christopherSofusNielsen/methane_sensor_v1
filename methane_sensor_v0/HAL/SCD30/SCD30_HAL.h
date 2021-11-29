/*
 * SCD30_HAL.h
 *
 * Created: 17-10-2021 15:11:43
 *  Author: Christopher S. Nielsen
 */ 


#ifndef SCD30_HAL_H_
#define SCD30_HAL_H_
#include <stdbool.h>

void SCD30_HAL_init();
bool SCD30_HAL_data_ready();



#endif /* SCD30_HAL_H_ */