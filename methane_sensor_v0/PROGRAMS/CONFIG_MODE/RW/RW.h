/*
 * RW.h
 *
 * Created: 09-11-2021 14:55:40
 *  Author: Mainframe
 */ 


#ifndef RW_H_
#define RW_H_

#include <stdbool.h>

#define RW_DEVEUI "deveui"
#define RW_APPEUI "appeui"
#define RW_APPKEY "appkey"
#define RW_PPM_FACTOR "ppmx"
#define RW_RRL "rrlx"
#define RW_VCC "vccx"
#define RW_COLS "cols"
#define RW_RESET "reset"

#define RW_S_SUCCESS "success"
#define RW_S_FAILED "failed"
#define RW_S_NOT_EXIST "Not a valid operation"

bool handle_read(const char cmd[], char res[]);
bool handle_write(const char cmd[], char res[]);



#endif /* RW_H_ */