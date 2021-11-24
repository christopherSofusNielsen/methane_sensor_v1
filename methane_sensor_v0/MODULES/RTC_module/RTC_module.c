/*
 * RTC_module.c
 *
 * Created: 02-11-2021 16:14:34
 *  Author: Christopher S. Nielsen
 */ 

#define decToBCD(n) (((n/10)<<4)|(n % 10))
#define BCDTodec(n) ((n >> 4)*10)+((n & 0x0F))

#include <xc.h>
#include "RTC_module.h"
#include "../../HAL/TWI/TWI_API.h"

/************************************************************************/
/* Typedef used to convert Datetime to MRPP ts                          */
/************************************************************************/
typedef union {
	uint8_t ts[4];
	uint32_t tsbit;
} tsbit;

/************************************************************************/
/* Constants I2C commands                                               */
/************************************************************************/
const uint8_t CMD_READ_TIME_POINTER[]={0x02};

/************************************************************************/
/* Local functions                                                      */
/************************************************************************/	
static void datetime_to_BCD(Datetime dt, DatetimeBCD *bcd);
//Currently not used
//static void BCD_to_datetime(DatetimeBCD bcd, Datetime *dt);
	
RTC_STATUS RTC_set_clock_out(uint8_t freq){
	uint8_t CMD_SET_CLOCK_OUT[2];
	CMD_SET_CLOCK_OUT[0]=0x0D;
	
	switch(freq){
		case 0:
			CMD_SET_CLOCK_OUT[1]=0x00;
			break;
		case 1:
			CMD_SET_CLOCK_OUT[1]=0x83;
			break;
		case 32:
			CMD_SET_CLOCK_OUT[1]=0x82;
			break;
		default:
			return RTC_STATUS_ERROR;
	}
	
	uint8_t status=TWI_API_write_data_stop(RTC_ADDR, CMD_SET_CLOCK_OUT, 2);
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

RTC_STATUS RTC_set_wake_up_interrupt_minutes(uint8_t minutes){
	uint8_t CMD_ENABLE_RTC_INT[]={0x01, 0b00000010};
	uint8_t CMD_SET_ALARM_TIME[]={0x09, 0x80, 0x80, 0x80, 0x80};
	
	//Read time
	Datetime dt;
	RTC_get_current_time(&dt);
	
	//add hours
	dt.minute += minutes;
	dt.minute %= 60;
	
	//Enable int
	uint8_t status = TWI_API_write_data_stop(RTC_ADDR, CMD_ENABLE_RTC_INT, 2);
	if(status != TWI_CODE_SUCCESS) return RTC_STATUS_FATAL_ERROR;
	
	//Set time
	CMD_SET_ALARM_TIME[1]=decToBCD(dt.minute);
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



void RTC_datetime_to_ts(Datetime dt, uint8_t ts[]){
	tsbit _tsbit;
	_tsbit.tsbit=0x00000000;
	_tsbit.tsbit |= ((uint32_t) dt.year)<<26;
	_tsbit.tsbit |= ((uint32_t) dt.month)<<22;
	_tsbit.tsbit |= ((uint32_t) dt.day)<<17;
	_tsbit.tsbit |= ((uint32_t) dt.hour)<<12;
	_tsbit.tsbit |= ((uint32_t) dt.minute)<<6;
	_tsbit.tsbit |= dt.second;
	
	ts[0]=_tsbit.ts[3];
	ts[1]=_tsbit.ts[2];
	ts[2]=_tsbit.ts[1];
	ts[3]=_tsbit.ts[0];
}

static void datetime_to_BCD(Datetime dt, DatetimeBCD *bcd){
	bcd->second=decToBCD(dt.second);
	bcd->minute=decToBCD(dt.minute);
	bcd->hour=decToBCD(dt.hour);
	bcd->day=decToBCD(dt.day);
	bcd->month=decToBCD(dt.month);
	bcd->year=decToBCD(dt.year);
}

//Not used
//static void BCD_to_datetime(DatetimeBCD bcd, Datetime *dt){
	//dt->second=BCDTodec(bcd.second);
	//dt->minute=BCDTodec(bcd.minute);
	//dt->hour=BCDTodec(bcd.hour);
	//dt->day=BCDTodec(bcd.day);
	//dt->month=BCDTodec(bcd.month);
	//dt->year=BCDTodec(bcd.year);
//}
