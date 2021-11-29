/*
 * RTC_module.h
 *
 * Created: 02-11-2021 16:14:44
 *  Author: Christopher S. Nielsen
 */ 


#ifndef RTC_MODULE_H_
#define RTC_MODULE_H_

//The I2C slave address of the RTC without the W/R bit
#define RTC_ADDR 0x51

#include <xc.h>

/************************************************************************/
/* Status Type                                                          */
/************************************************************************/
typedef enum {
	RTC_STATUS_SUCCESS,
	RTC_STATUS_ERROR,
	RTC_STATUS_FATAL_ERROR,
	RTC_STATUS_TRY_AGAIN
}RTC_STATUS;

/************************************************************************/
/* Datetimes struct and Datetimes in BCD format struct.                 */
/************************************************************************/
typedef struct Datetimes{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint8_t year;
}Datetime;

typedef struct DatetimesBCD{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint8_t year;
}DatetimeBCD;

/************************************************************************
* Sets the clock out frequency
* -> uint8_t freq: The frequency, either 0 (stopped), 1 or 32
* -> Returns RTC_STATUS_SUCCESS when successfull, 
	 otherwise RTC_STATUS_ERROR when incorrect frequency
	 or RTC_STATUS_FATAL_ERROR on error.
************************************************************************/
RTC_STATUS RTC_set_clock_out(uint8_t freq);

/************************************************************************
* Gets the current time
* -> Datetime *dt: Pointer to a Datetime struct where the time must be stored.
* -> Returns RTC_STATUS_SUCCESS when successfull, otherwise RTC_STATUS_FATAL_ERROR
************************************************************************/
RTC_STATUS RTC_get_current_time(Datetime *dt);

/************************************************************************
* Sets the current time
* -> Datetime dt: Datetime struct containing the time that should be set
* -> Returns RTC_STATUS_SUCCESS when successfull, otherwise RTC_STATUS_FATAL_ERROR
************************************************************************/
RTC_STATUS RTC_set_current_time(Datetime dt);

/************************************************************************
* Sets the wake-up interrupt
* -> uint8_t hours: The hours from now, the intterupt should occur. 
*	 (Works on hole hours only, fx time now is 3:22 and hours=2, then interrupt will occur 5:00)
* -> Returns RTC_STATUS_SUCCESS when successfull, otherwise RTC_STATUS_FATAL_ERROR
************************************************************************/
RTC_STATUS RTC_set_wake_up_interrupt(uint8_t hours);

/************************************************************************/
/* Same as RTC_set_wake_up_interrupt, just in minutes.					*/
/************************************************************************/
RTC_STATUS RTC_set_wake_up_interrupt_minutes(uint8_t minutes);

/************************************************************************
* Clears the wake-up interrupt
* -> Returns RTC_STATUS_SUCCESS when successfull, otherwise RTC_STATUS_FATAL_ERROR
************************************************************************/
RTC_STATUS RTC_clear_wake_up_interrupt();

/************************************************************************
* Converts Datetime struct to MRPP ts array
* -> Datetime dt: Datetime struct containing the time
* -> uint8_t ts[]: Pointer to a 4 byte long array the timestamp must be stored
************************************************************************/
void RTC_datetime_to_ts(Datetime dt, uint8_t ts[]);


#endif /* RTC_MODULE_H_ */