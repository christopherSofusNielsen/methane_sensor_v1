/*
 * CONFIG_MODE.h
 *
 * Created: 09-11-2021 13:35:25
 *  Author: Christopher S. Nielsen
 */ 


#ifndef CONFIG_MODE_H_
#define CONFIG_MODE_H_

/************************************************************************/
/* States in config state machine                                       */
/************************************************************************/
typedef enum{
	CONF_INTRO,
	CONF_WAIT_FOR_CMD,
	CONF_CLEAR_FOR_NEW_CMD,
	CONF_PARSE,
	CONF_EXIT,
	CONF_NOT_VALID,
	CONF_FORMAT_ERR,
	CONF_READ,
	CONF_WRTIE,
	CONF_CONNECT,
	CONF_SAMPLE,
	CONF_REPLY,
	
} CONF_STATES;


void CONF_enter();



#endif /* CONFIG_MODE_H_ */