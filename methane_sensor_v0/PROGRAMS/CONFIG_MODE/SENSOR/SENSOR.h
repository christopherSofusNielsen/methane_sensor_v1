/*
 * SAMPLE.h
 *
 * Created: 11-11-2021 14:55:05
 *  Author: Mainframe
 */ 


#ifndef SAMPLE_H_
#define SAMPLE_H_

#define S_N_SAMPLES 20

#define S_METH "meth"
#define S_CO2 "co2"
#define S_PUMP	"pump"

#define METH_POWER_ON "on"
#define METH_POWER_OFF "off"
#define METH_READ "read"

#define CO2_READ "read"

#define PUMP_ON "on"
#define PUMP_OFF "off"


#include <xc.h>
#include <stdbool.h>

bool handle_sensor(const char cmd[], char res[]);
bool init_methane_SCD30();


//void sample_methane();






#endif /* SAMPLE_H_ */