/*
 * EEPROM_module.c
 *
 * Created: 18-10-2021 13:08:12
 *  Author: Christopher S. Nielsen
 */ 

#include "EEPROM_module.h"
#include <avr/eeprom.h>
#include <stdbool.h>
#include <string.h>


void EM_clear_lora_prameters(){
	eeprom_update_byte((uint8_t *) EEPROM_HAS_DEVEUI_ADDR, false);
	eeprom_update_byte((uint8_t *) EEPROM_HAS_APPEUI_ADDR, false);
	eeprom_update_byte((uint8_t *) EEPROM_HAS_APPKEY_ADDR, false);
}

bool EM_has_deveui(){
	return eeprom_read_byte((uint8_t *) EEPROM_HAS_DEVEUI_ADDR);
}

bool EM_has_appeui(){
	return eeprom_read_byte((uint8_t *) EEPROM_HAS_APPEUI_ADDR);
}

bool EM_has_appkey(){
	return eeprom_read_byte((uint8_t *) EEPROM_HAS_APPKEY_ADDR);
}

bool EM_set_deveui(const char deveui[]){
	if(strlen(deveui)!=16) return false;
	eeprom_update_block((const void *) deveui, (void *) EEPROM_DEVEUI_ADDR, EEPROM_DEVEUI_LENGTH );
	eeprom_update_byte((uint8_t *) EEPROM_HAS_DEVEUI_ADDR, true);
	return true;
}

bool EM_get_deveui(char deveui[]){
	if(!EM_has_deveui()) return false;
	eeprom_read_block((void *) deveui, ( const void *) EEPROM_DEVEUI_ADDR, EEPROM_DEVEUI_LENGTH);
	deveui[EEPROM_DEVEUI_LENGTH]='\0';
	return true;	
}

bool EM_set_appeui(const char appeui[]){
	if(strlen(appeui)!=16) return false;
	eeprom_update_block((const void *) appeui, (void *) EEPROM_APPEUI_ADDR, EEPROM_APPEUI_LENGTH );
	eeprom_update_byte((uint8_t *) EEPROM_HAS_APPEUI_ADDR, true);
	return true;
}

bool EM_get_appeui(char appeui[]){
	if(!EM_has_appeui()) return false;
	eeprom_read_block((void *) appeui, ( const void *) EEPROM_APPEUI_ADDR, EEPROM_APPEUI_LENGTH);
	appeui[EEPROM_APPEUI_LENGTH]='\0';
	return true;
}

bool EM_set_appkey(const char appkey[]){
	if(strlen(appkey)!=32) return false;
	eeprom_update_block((const void *) appkey, (void *) EEPROM_APPKEY_ADDR, EEPROM_APPKEY_LENGTH );
	eeprom_update_byte((uint8_t *) EEPROM_HAS_APPKEY_ADDR, true);
	return true;
}

bool EM_get_appkey(char appkey[]){
	if(!EM_has_appkey()) return false;
	eeprom_read_block((void *) appkey, ( const void *) EEPROM_APPKEY_ADDR, EEPROM_APPKEY_LENGTH);
	appkey[EEPROM_APPKEY_LENGTH]='\0';
	return true;
}

void EM_set_ppmfactor(float val){
	eeprom_update_float((float *) EEPROM_PPM_FACTOR_ADDR, val);
}

void EM_get_ppmfactor(float *val){
	*val=eeprom_read_float((const float *) EEPROM_PPM_FACTOR_ADDR);
}

void EM_set_RRL(float val){
	eeprom_update_float((float *) EEPROM_RRL_ADDR, val);
}

void EM_get_RRL(float *val){
	*val=eeprom_read_float((const float *) EEPROM_RRL_ADDR);
}

void EM_set_Vcc(float val){
	eeprom_update_float((float *) EEPROM_VCC_ADDR, val);
}

void EM_get_Vcc(float *val){
	*val=eeprom_read_float((const float *) EEPROM_VCC_ADDR);
}

void EM_set_collections(const void *cols, uint8_t size){
	eeprom_update_block(cols, (void *) EEPROM_COLLECTION_ADDR, size );
}

void EM_get_collections(void *cols, uint8_t size){
	eeprom_read_block(cols, ( const void *) EEPROM_COLLECTION_ADDR, size);
}

void EM_set_heat_up_time(uint8_t time){
	eeprom_update_byte((uint8_t *) EEPROM_HEAT_UP_T_ADDR, time);
}

uint8_t EM_get_heat_up_time(){
	return eeprom_read_byte((const uint8_t *) EEPROM_HEAT_UP_T_ADDR);
}

void EM_set_air_pump_time(uint16_t time){
	eeprom_update_word((uint16_t *) EEPROM_AIR_PUMP_T_ADDR, time);
}

uint16_t EM_get_air_pump_time(){
	return eeprom_read_word((const uint16_t *) EEPROM_AIR_PUMP_T_ADDR);
}

void EM_set_sp_interval(uint8_t time){
	eeprom_update_byte((uint8_t *) EEPROM_SP_INTERVAL_ADDR, time);
}
uint8_t EM_get_sp_interval(){
	return eeprom_read_byte((const uint8_t *) EEPROM_SP_INTERVAL_ADDR);
}

void EM_set_group_id(uint8_t groupId){
	eeprom_update_byte((uint8_t *) EEPROM_GROUP_ID_ADDR, groupId);
}

uint8_t EM_get_group_id(){
	return eeprom_read_byte((const uint8_t *) EEPROM_GROUP_ID_ADDR);
}