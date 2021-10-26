/*
 * EEPROM_module.c
 *
 * Created: 18-10-2021 13:08:12
 *  Author: Mainframe
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
}

bool EM_get_appkey(char appkey[]){
	if(!EM_has_appkey()) return false;
	eeprom_read_block((void *) appkey, ( const void *) EEPROM_APPKEY_ADDR, EEPROM_APPKEY_LENGTH);
	appkey[EEPROM_APPKEY_LENGTH]='\0';
	return true;
}