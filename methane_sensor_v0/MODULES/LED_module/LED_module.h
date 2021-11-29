/*
 * LED_module.h
 *
 * Created: 08-11-2021 21:54:04
 *  Author: Christopher S. Nielsen
 */ 


#ifndef LED_MODULE_H_
#define LED_MODULE_H_

/************************************************************************/
/* Blink join-sequence (interrupt based)                                */
/************************************************************************/
void LED_start_try_join();

/************************************************************************/
/* Stop blink join-sequence                                             */
/************************************************************************/
void LED_stop_try_join();

/************************************************************************/
/* Blink status codes                                                   */
/************************************************************************/
void LED_join_success();
void LED_join_denied();
void LED_conf_err();
void LED_fatal_err();


#endif /* LED_MODULE_H_ */