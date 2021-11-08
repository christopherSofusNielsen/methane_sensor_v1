/*
 * SCD30_module.h
 *
 * Created: 17-10-2021 14:53:10
 *  Author: Mainframe
 */ 


#ifndef SCD30_MODULE_H_
#define SCD30_MODULE_H_

#include <xc.h>
#include <stdbool.h>

typedef enum {
	SCD30_STATUS_SUCCESS,
	SCD30_STATUS_ERROR,
	SCD30_STATUS_FATAL_ERROR,
	SCD30_STATUS_TRY_AGAIN
}SCD30_STATUS;

SCD30_STATUS SCD30_init_sampling(uint16_t samplingInterval, uint16_t nSamples, uint16_t data[]);
void SCD30_deinit_sampling();
SCD30_STATUS SCD30_start_sampling();
SCD30_STATUS SCD30_get_reading(uint16_t *value);
bool SCD30_is_sampling_done();
bool SCD30_data_ready();



#endif /* SCD30_MODULE_H_ */