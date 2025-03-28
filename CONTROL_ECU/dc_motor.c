 /******************************************************************************
 * dc_motor.c
 *
 *  Created on: Oct 3, 2024
 *      Author: Malak
 *******************************************************************************/

#include "dc_motor.h"
#include "pwm.h"
#include "gpio.h"
#include "common_macros.h"

uint8 g_dcMotorFanState = 0; /* Global Variable to determine the fan's state */

/*
 * Description :
 * Initialize the DC motor:
 * 1. Setup the DC motor pins direction using GPIO driver.
 * 2. Turn Off the DC motor as startup using GPIO driver.
 */
void DcMotor_init(void){
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID, DC_MOTOR_IN1_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID, DC_MOTOR_IN2_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID, DC_MOTOR_ENABLE_ID, PIN_OUTPUT);
	/* DC_MOTOR_ENABLE_ID is already initialized in PWM*/

	/* Stooping the motor at the beginning */
	GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_IN1_ID, LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_IN2_ID, LOGIC_LOW);
}

/*
 * Description :
 * Activate the DC motor:
 * 1. Write on the DC motor pins based on the input informations using GPIO driver.
 * 2. Start the PWM.
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed){
	g_dcMotorFanState = state;

	if(state == CLOCKWISE)
	{
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_IN1_ID, LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_IN2_ID, LOGIC_LOW);

		PWM_Timer0_Start(speed);
	}
	else if(state == ANTI_CLOCKWISE)
	{
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_IN1_ID, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_IN2_ID, LOGIC_HIGH);

		PWM_Timer0_Start(speed);
	}
	else if(state == STOP)
	{
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_IN1_ID, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_IN2_ID, LOGIC_LOW);
	}
}


