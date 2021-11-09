/*
 * RW.h
 *
 * Created: 09-11-2021 14:55:40
 *  Author: Mainframe
 */ 


#ifndef RW_H_
#define RW_H_

#include <stdbool.h>

bool handle_read(const char cmd[], char res[]);
bool handle_write(const char cmd[], char res[]);



#endif /* RW_H_ */