 /******************************************************************************
 * buzzer.c
 *
 *  Created on: Oct 3, 2024
 *      Author: Malak
 *******************************************************************************/
#include "buzzer.h"
#include "gpio.h"

/*
 * Description :
 * Initialize the Buzzer:
 * 1. Initialize the output pin of the buzzer.
 * 2. Write 0 to the buzzer pin to turn it off.
 */
void BUZZER_init(void)
{
	GPIO_setupPinDirection(BUZZER_PORT_OUTPUT_ID, BUZZER_PIN_OUTPUT_ID, PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_OUTPUT_ID, BUZZER_PIN_OUTPUT_ID, BUZZER_OFF);
}

/*
 * Description :
 * Write 1 to the buzzer pin to turn it on.
 */
void BUZZER_on(void)
{
	GPIO_writePin(BUZZER_PORT_OUTPUT_ID, BUZZER_PIN_OUTPUT_ID, BUZZER_ON);
}

/*
 * Description :
 * Write 0 to the buzzer pin to turn it on.
 */
void BUZZER_off(void)
{
	GPIO_writePin(BUZZER_PORT_OUTPUT_ID, BUZZER_PIN_OUTPUT_ID, BUZZER_OFF);
}
