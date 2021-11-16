/*
 * util.h
 *
 * Created: 05-10-2021 16:03:27
 *  Author: Mainframe
 */ 


#ifndef UTIL_H_
#define UTIL_H_

#include <stdbool.h>

#include "../lora_module.h"

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
bool util_strbeginswith(const char *s1, const char *s2);
LM_STATUS util_reset_module();
void util_lora_forward_msg(const char msg[], char res[]);
void util_send_break(char res[]);


#endif /* UTIL_H_ */