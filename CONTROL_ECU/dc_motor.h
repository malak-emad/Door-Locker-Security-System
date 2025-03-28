 /******************************************************************************
 * dc_motor.h
 *
 *  Created on: Oct 3, 2024
 *      Author: Malak
 *******************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define DC_MOTOR_PORT_ID		PORTD_ID
#define DC_MOTOR_IN1_ID			PIN6_ID
#define DC_MOTOR_IN2_ID			PIN7_ID
#define DC_MOTOR_ENABLE_PORT_ID PORTB_ID
#define DC_MOTOR_ENABLE_ID		PIN3_ID

//uint8 g_fanState = 0; /* Global Variable to determine the fan's state */

typedef enum{
	STOP, CLOCKWISE, ANTI_CLOCKWISE
}DcMotor_State;


/*******************************************************************************
 *                             Function Prototypes                             *
 *******************************************************************************/

/*
 * Description :
 * Initialize the DC motor.
 */
void DcMotor_init(void);

/*
 * Description :
 * Activate the DC motor.
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed);

#endif /* DC_MOTOR_H_ */
