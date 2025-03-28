 /******************************************************************************
 * pwm.c
 *
 *  Created on: Oct 3, 2024
 *      Author: Malak
 *******************************************************************************/

#include <avr/io.h>
#include "pwm.h"
#include "gpio.h"
#include "common_macros.h"

void PWM_Timer0_Start(uint8 duty_cycle)
{

	/* TCNT0 Register is set to 0 as an initialized value
	 */
	TCNT0 = 0;

	/* TCCR0 Register Bits Description:
	 * FOC0 = 0 to specify the PWM mode
	 * WGM01:0 = 11 to choose Fast PWM mode
	 * COM01:0 = 10 to choose non-inverting mode
	 * CS02:0  = 101 to choose Prescaler: F_CPU/1024
	 */
	SET_BIT(TCCR0, WGM00);
	SET_BIT(TCCR0, WGM01);
	SET_BIT(TCCR0, COM01);
	SET_BIT(TCCR0, CS02);
	SET_BIT(TCCR0, CS00);

	/*duty_cycle: Percentage (0 to 100%) representing the PWM duty cycle.
	 */
	OCR0 = (uint8)(((float32)(duty_cycle / 100.0)) * 255.0);

	/* Set the pin direction as output using GPIO driver
	 */
	GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);

}

