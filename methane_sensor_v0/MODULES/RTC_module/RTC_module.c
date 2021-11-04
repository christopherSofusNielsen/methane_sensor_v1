/*
 * RTC_module.c
 *
 * Created: 02-11-2021 16:14:34
 *  Author: Mainframe
 */ 

#define decToBCD(n) (((n/10)<<4)|(n % 10))
#define BCDTodec(n) ((n >> 4)*10)+((n & 0x0F))

#include <xc.h>
#include "RTC_module.h"
#include "../../HAL/TWI/TWI_API.h"
#include "../../HAL/TWI/TWI_HAL.h"

#define RTC_ADDR 0x51

const uint8_t CMD_READ_TIME_POINTER[]={0x02};
	
RTC_STATUS RTC_set_wake_up_interrupt(uint8_t hours){
	uint8_t CMD_ENABLE_RTC_INT[]={0x01, 0b00000010};
	uint8_t CMD_SET_ALARM_TIME[]={0x09, 0x80, 0x80, 0x80, 0x80};
	
	//Read time 
	Datetime dt;
	RTC_get_current_time(&dt);
	
	//add hours
	dt.hour += hours;
	dt.hour %= 24;
	
	//Enable int
	uint8_t status = TWI_API_write_data_stop(RTC_ADDR, CMD_ENABLE_RTC_INT, 2);
	if(status != TWI_CODE_SUCCESS) return RTC_STATUS_FATAL_ERROR;
	
	//Set time
	CMD_SET_ALARM_TIME[2]=decToBCD(dt.hour);
	status = TWI_API_write_data_stop(RTC_ADDR, CMD_SET_ALARM_TIME, 5);
	if(status != TWI_CODE_SUCCESS) return RTC_STATUS_FATAL_ERROR;
	
	return RTC_STATUS_SUCCESS;
}

RTC_STATUS RTC_clear_wake_up_interrupt(){
	uint8_t CMD_ENABLE_RTC_CLEAR_INT[]={0x01, 0b00000000};
	uint8_t status = TWI_API_write_data_stop(RTC_ADDR, CMD_ENABLE_RTC_CLEAR_INT, 2);
	if(status != TWI_CODE_SUCCESS) return RTC_STATUS_FATAL_ERROR;
	return RTC_STATUS_SUCCESS;
}

RTC_STATUS RTC_get_current_time(Datetime *dt){
	uint8_t data[7];
	uint8_t status;
	
	status = TWI_API_write_data(RTC_ADDR, CMD_READ_TIME_POINTER, 1);
	if(status != TWI_CODE_SUCCESS) return RTC_STATUS_FATAL_ERROR;
	
 	status = TWI_API_repeat_read_data_stop(RTC_ADDR, data, 7);
	if(status != TWI_CODE_SUCCESS) return RTC_STATUS_FATAL_ERROR;
	
	dt->second=BCDTodec((data[0] & 0x7F));
	dt->minute=BCDTodec((data[1] & 0x7F));
	dt->hour=BCDTodec((data[2] & 0x3F));
	dt->day=BCDTodec((data[3] & 0x3F));
	dt->month=BCDTodec((data[5] & 0x1F));
	dt->year=BCDTodec((data[6] & 0xFF));
	
	return RTC_STATUS_SUCCESS;
}

RTC_STATUS RTC_set_current_time(Datetime dt){
	uint8_t data [8];
	DatetimeBCD bcd;
	datetime_to_BCD(dt, &bcd);
	
	data[0]=0x02;
	data[1]=bcd.second;
	data[2]=bcd.minute;
	data[3]=bcd.hour;
	data[4]=bcd.day;
	data[5]=0x00;
	data[6]=bcd.month;
	data[7]=bcd.year;
	
	uint8_t status=TWI_API_write_data_stop(RTC_ADDR, data, 8);
	if(status!=TWI_CODE_SUCCESS) return RTC_STATUS_FATAL_ERROR;
	
	return RTC_STATUS_SUCCESS;
}




void datetime_to_BCD(Datetime dt, DatetimeBCD *bcd){
	bcd->second=decToBCD(dt.second);
	bcd->minute=decToBCD(dt.minute);
	bcd->hour=decToBCD(dt.hour);
	bcd->day=decToBCD(dt.day);
	bcd->month=decToBCD(dt.month);
	bcd->year=decToBCD(dt.year);
}

void BCD_to_datetime(DatetimeBCD bcd, Datetime *dt){
	dt->second=BCDTodec(bcd.second);
	dt->minute=BCDTodec(bcd.minute);
	dt->hour=BCDTodec(bcd.hour);
	dt->day=BCDTodec(bcd.day);
	dt->month=BCDTodec(bcd.month);
	dt->year=BCDTodec(bcd.year);
}
