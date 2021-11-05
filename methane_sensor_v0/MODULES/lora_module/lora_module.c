/*
 * lora_module.c
 *
 * Created: 05-10-2021 15:57:06
 *  Author: Mainframe
 */ 

#include <xc.h>
#include <stdbool.h>

#include "lora_module.h"
#include "join_network/join_network.h"
#include "send_uplink/send_uplink.h"
#include "sleep_controller/sleep_controller.h"
#include "../../HAL/TC2/TC2_HAL.h"

static uint8_t cnt=0;
static bool busy=false;
static int8_t subPointer;

static void block_uplink();
static void set_block_uplink();

LM_STATUS LM_join_network(char deveui[], char appeui[], char appkey[]){
	return JN_join_network(deveui, appeui, appkey);
}

LM_STATUS LM_send_uplink(uint8_t data[], uint8_t length){
	if(busy) return LM_STATUS_TRY_AGAIN;
	set_block_uplink();
	return SU_send_uplink(LORA_PORT, data, length);
}

LM_STATUS LM_put_to_sleep(){
	return SC_put_to_sleep();
}

LM_STATUS LM_wake_up(){
	return SC_wake_up();
}

bool LM_is_free(){
	return busy==false;
}

static void set_block_uplink(){
	subPointer=TC2_HAL_subscribe(&block_uplink);
	busy=true;
	cnt=0;
}

static void block_uplink(){
	cnt++;
	if(cnt>LORA_UPLINK_BLOCK_TIME){
		busy=false;
		TC2_HAL_cancel(subPointer);
	}
}