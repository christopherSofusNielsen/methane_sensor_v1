/*
 * TC2_HAL.c
 *
 * Created: 04-11-2021 19:00:03
 *  Author: Mainframe
 */ 

#include <xc.h>
#include <avr/interrupt.h>
#include <stddef.h>

#include "TC2_HAL.h"
#include "../types.h"
#include "../../util/bit_operators.h"

static void start_timer();
static void stop_timer();
static void clear_callbacks();

static callback_ptr callbacks[MAX_CALLBACKS];
static uint8_t cbCounter=0;
static uint8_t cnt=0;

void TC2_HAL_init(){
	TCCR2A=0x02; //Set mode=CTC
	TCCR2B=0x00; //Stop timer
	
	//Disable async
	ASSR=0x00;
	
	TCNT2=0; //Set counter to 0
	OCR2A=251; //15 Hz, 31 event pr s
	
	//Enable interrupt
	TIMSK2=0x02;
	sei();
}

static void clear_callbacks(){
	for (uint8_t i=0; i<MAX_CALLBACKS; i++)
	{
		callbacks[i]=NULL;
	}
	cbCounter=0;
}

static void start_timer(){
	TCCR2B=0x07;
}

static void stop_timer(){
	TCCR2B=0x00;
}

void TC2_HAL_kill(){
	stop_timer();
	clear_callbacks();
}

int8_t TC2_HAL_subscribe(callback_ptr cb){
	int8_t pointer=-1;
	
	if(cbCounter>= MAX_CALLBACKS) return pointer;
	
	for (uint8_t i=0; i<MAX_CALLBACKS; i++)
	{
		if(callbacks[i]==NULL){
			callbacks[i]=cb;
			pointer=i;
			break;
		}
	}
	cbCounter++;
	start_timer();
	return pointer;
}

void TC2_HAL_cancel(int8_t pointer){
	if(callbacks[pointer]==NULL) return;
	callbacks[pointer]=NULL;
	cbCounter--;
	if(cbCounter==0){
		stop_timer();		
	}
}

ISR(TIMER2_COMPA_vect){
	cnt++;
	
	if(cnt % 31==0){
		cnt=0;
		for (uint8_t i=0; i<MAX_CALLBACKS; i++)
		{
			if(callbacks[i]!=NULL){
				callbacks[i]();
			}
		}
	}
}

