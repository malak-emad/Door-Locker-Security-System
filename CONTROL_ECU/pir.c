/*
 * pir.c
 *
 *  Created on: Nov 2, 2024
 *      Author: Malak
 */
#include "pir.h"
#include "gpio.h"
#include "common_macros.h"

/* Description: Function to initialize the PIR driver
   Inputs: None
   Return: None */
void PIR_init(void)
{
	GPIO_setupPinDirection(PIR_PORT_ID, PIR_PIN_ID, PIN_INPUT);
}

/* Description: Function to return PIR State
   Inputs: None
   Return: uint8 */
uint8 PIR_getState(void)
{
	return (GPIO_readPin(PIR_PORT_ID, PIR_PIN_ID));
}

