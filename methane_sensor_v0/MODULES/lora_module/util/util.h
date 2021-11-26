/*
 * util.h
 *
 * Created: 05-10-2021 16:03:27
 *  Author: Christopher S. Nielsen
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


void util_set_pending_states(LM_STATE_DATA *sd, int success, int failed);
void util_set_match_case(LM_STATE_DATA *sd, LM_MATCH_CASES matchCase, const char *matchString);
int util_match_message(LM_STATE_DATA *sd, char *msg);
bool util_str_begins_with(const char *s1, const char *s2);
LM_STATUS util_reset_module();
void util_lora_forward_msg(const char msg[], char res[]);
void util_send_break();


#endif /* UTIL_H_ */