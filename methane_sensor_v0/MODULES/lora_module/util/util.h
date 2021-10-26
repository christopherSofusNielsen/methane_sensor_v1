/*
 * util.h
 *
 * Created: 05-10-2021 16:03:27
 *  Author: Mainframe
 */ 


#ifndef UTIL_H_
#define UTIL_H_


#define __DEVEUI "0004A30B00F4547A"
#define __APPKEY "B88AD6D25A3B27C69A01F74C53F9A179"
#define __APPEUI "70B3D57ED003F844"



typedef enum {LM_EXACT, LM_BEGINS_WITH, LM_NO_TEST} LM_MATCH_CASES;

typedef struct __STATE_DATA{
	int success_state;
	int failure_state;
	LM_MATCH_CASES matchCase;
	const char *matchString;
}LM_STATE_DATA;

extern char lm_msg[200];


void util_setPendingStates(LM_STATE_DATA *sd, int success, int failed);
void util_setMatchCase(LM_STATE_DATA *sd, LM_MATCH_CASES matchCase, const char *matchString);
int util_matchMessage(LM_STATE_DATA *sd, char *msg);
void util_transmit_msg(char msg[]);
void util_read_msg(char msg[]);
void util_get_deveui_as_string(char deveui[]);
void util_get_appkey_as_string(char appkey[]);
void util_get_appeui_as_string(char appeui[]);


#endif /* UTIL_H_ */