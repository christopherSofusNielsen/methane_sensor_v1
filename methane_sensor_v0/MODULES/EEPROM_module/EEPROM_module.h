/*
 * EEPROM_module.h
 *
 * Created: 18-10-2021 13:08:01
 *  Author: Mainframe
 */ 


#ifndef EEPROM_MODULE_H_
#define EEPROM_MODULE_H_
#include <stdbool.h>


/************************************************************************/
/* Address                                                              */
/************************************************************************/
#define EEPROM_DEVEUI_ADDR 0
#define EEPROM_DEVEUI_LENGTH 16
#define EEPROM_APPEUI_ADDR 16
#define EEPROM_APPEUI_LENGTH 16
#define EEPROM_APPKEY_ADDR 32
#define EEPROM_APPKEY_LENGTH 32
#define EEPROM_HAS_DEVEUI_ADDR 64
#define EEPROM_HAS_APPEUI_ADDR 65
#define EEPROM_HAS_APPKEY_ADDR 66

/************************************************************************/
/* Address                                                              */
/************************************************************************/


void EM_clear_lora_prameters();
bool EM_has_deveui();
bool EM_has_appeui();
bool EM_has_appkey();

bool EM_set_deveui(const char deveui[]);
bool EM_get_deveui(char deveui[]);

bool EM_set_appeui(const char appeui[]);
bool EM_get_appeui(char appeui[]);

bool EM_set_appkey(const char appkey[]);
bool EM_get_appkey(char appkey[]);



#endif /* EEPROM_MODULE_H_ */