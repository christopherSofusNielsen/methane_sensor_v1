/*
 * RN2483_protocol.h
 *
 * Created: 04-10-2021 17:12:30
 *  Author: Mainframe
 */ 


#ifndef RN2483_PROTOCOL_H_
#define RN2483_PROTOCOL_H_

#include <xc.h>

//Constants
#define MAX_PAYLOAD_SIZE_EU868 50
#define PORT_MIN 1
#define PORT_MAX 223

//Others
#define CMD_END_CHARS "\r\n"
#define RN2483 "RN2483"

//Status codes
#define OK "ok"
#define BUSY "busy"
#define FRAM_COUNTER_ERR_REJOIN_NEEDED "fram_counter_err_rejoin_needed"
#define INVALID_CLASS "invalid_class"
#define INVALID_DATA_LEN "invalid_data_len"
#define INVALID_PARAM "invalid_param"
#define KEYS_NOT_INIT "keys_not_init"
#define MAC_PAUSED "mac_paused"
#define MULTICAST_KEYS_NOT_SET "multicast_keys_not_set"
#define NO_FREE_CH "no_free_ch"
#define NOT_JOINED "not_joined"
#define SILENT "silent"
#define ERR "err"
#define ACCEPTED "accepted"
#define DENIED "denied"
#define MAC_TX_OK "mac_tx_ok"
#define MAC_RX "mac_rx"
#define MAC_ERR "mac_err"
#define ON "on"
#define OFF "off"

//Sys commands
#define SYS_SLEEP "sys sleep"
#define SYS_GET_VERSION_CMD "sys get ver"
#define SYS_GET_HW_EUI_CMD "sys get hweui"

//Mac commands
#define MAC_RESET_TO_868_BAND "mac reset 868"
#define MAC_RESET_TO_433_BAND "mac reset 433"
#define SET_DEVEUI "mac set deveui"
#define SET_APPEUI "mac set appeui"
#define SET_APPKEY "mac set appkey"
#define MAC_TX_CONFIRMED "mac tx cnf"
#define MAC_JOIN_OTAA "mac join otaa"
#define MAC_SAVE "mac save"
#define  SET_ADR "mac set adr"
//#define SET_ADR_OFF "mac set adr off"
//#define SET_ADR_ON "mac set adr on"


//Functions
void attach_ending(char buffer[]);
int16_t cmd_with_ending(char buffer[], const char cmd[]);
int16_t sys_sleep_cmd(char buffer[], uint32_t time);
int16_t mac_set_parameter(char buffer[], const char *cmd, const char *parameter);
int16_t mac_set_deveui(char buffer[], char *eui);
int16_t mac_set_appeui(char buffer[], char *appeui);
int16_t mac_set_appkey(char buffer[], char *appkey);
int16_t mac_set_ADR(char buffer[], unsigned char state);
int16_t mac_tx_confirmed(char buffer[], uint8_t port, uint8_t data[], uint8_t bytes);



#endif /* RN2483_PROTOCOL_H_ */