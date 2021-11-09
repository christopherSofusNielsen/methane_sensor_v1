/*
 * LED_module.h
 *
 * Created: 08-11-2021 21:54:04
 *  Author: Mainframe
 */ 


#ifndef LED_MODULE_H_
#define LED_MODULE_H_

void LED_start_try_join();
void LED_stop_try_join();

void LED_join_success();
void LED_join_denied();
void LED_join_conf_err();
void LED_fatal_err();


#endif /* LED_MODULE_H_ */