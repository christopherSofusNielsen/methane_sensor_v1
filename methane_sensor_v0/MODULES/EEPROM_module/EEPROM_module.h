/*
 * EEPROM_module.h
 *
 * Created: 18-10-2021 13:08:01
 *  Author: Mainframe
 */ 


#ifndef EEPROM_MODULE_H_
#define EEPROM_MODULE_H_
#include <stdbool.h>
#include <xc.h>


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
#define EEPROM_PPM_FACTOR_ADDR 67 //size 4 bytes
#define EEPROM_RRL_ADDR 71 //size 4 bytes
#define EEPROM_VCC_ADDR 75 //size 4 bytes
#define EEPROM_HEAT_UP_T_ADDR 79 //size 1 byte
#define EEPROM_AIR_PUMP_T_ADDR 80 //size 2 bytes
#define EEPROM_SP_INTERVAL_ADDR 82 //size 1 byte

//Notice jump in addr
#define EEPROM_GROUP_ID_ADDR 99 //Size 1 byte
#define EEPROM_COLLECTION_ADDR 100 //11 collections=11*5=55 bytes

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

void EM_set_ppmfactor(float val);
void EM_get_ppmfactor(float *val);

void EM_set_RRL(float val);
void EM_get_RRL(float *val);

void EM_set_Vcc(float val);
void EM_get_Vcc(float *val);

void EM_set_collections(const void *cols, uint8_t size);
void EM_get_collections(void *cols, uint8_t size);

void EM_set_heat_up_time(uint8_t time);
uint8_t EM_get_heat_up_time();

void EM_set_air_pump_time(uint16_t time);
uint16_t EM_get_air_pump_time();

void EM_set_sp_interval(uint8_t time);
uint8_t EM_get_sp_interval();

void EM_set_group_id(uint8_t groupId);
uint8_t EM_get_group_id();



#endif /* EEPROM_MODULE_H_ */