/*
 * RTC_module.h
 *
 * Created: 02-11-2021 16:14:44
 *  Author: Mainframe
 */ 


#ifndef RTC_MODULE_H_
#define RTC_MODULE_H_

#include <xc.h>

typedef enum {
	RTC_STATUS_SUCCESS,
	RTC_STATUS_ERROR,
	RTC_STATUS_FATAL_ERROR,
	RTC_STATUS_TRY_AGAIN
}RTC_STATUS;

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


RTC_STATUS RTC_set_clock_out(uint8_t freq);
RTC_STATUS RTC_get_current_time(Datetime *dt);
RTC_STATUS RTC_set_current_time(Datetime dt);
RTC_STATUS RTC_set_wake_up_interrupt(uint8_t hours);
RTC_STATUS RTC_clear_wake_up_interrupt();
RTC_STATUS RTC_set_wake_up_interrupt_minutes(uint8_t minutes);
void RTC_datetime_to_ts(Datetime dt, uint8_t ts[]);


#endif /* RTC_MODULE_H_ */