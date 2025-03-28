/*
 * pir.h
 *
 *  Created on: Nov 2, 2024
 *      Author: Malak
 */

#ifndef PIR_H_
#define PIR_H_

#include "gpio.h"

#define PIR_PORT_ID   		    PORTC_ID
#define PIR_PIN_ID			    PIN2_ID
#define PIR_NO_MOTION_DETECTED  0
#define PIR_MOTION_DETECTED  	1

/* Description: Function to initialize the PIR driver
   Inputs: None
   Return: None */
void PIR_init(void);

/* Description: Function to return PIR State
   Inputs: None
   Return: uint8 */
uint8 PIR_getState(void);

#endif /* PIR_H_ */
