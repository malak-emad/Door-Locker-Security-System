/*
 * keypad.h
 *
 *  Created on: Sep 27, 2024
 *      Author: Malak
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define KEYPAD_COLUMNS_NUMBER		4
#define KEYPAD_ROWS_NUMBER			4

#define KEYPAD_ROW_PORT_ID			PORTB_ID
#define KEYPAD_ROW_FIRST_PIN_ID		PIN0_ID

#define KEYPAD_COL_PORT_ID			PORTB_ID
#define KEYPAD_COL_FIRST_PIN_ID		PIN4_ID

#define KEYPAD_BUTTON_PRESSED		LOGIC_LOW
#define KEYPAD_BUTTON_RELEASED		LOGIC_HIGH


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Get the Keypad pressed button
 */
uint8 KEYPAD_getPressedKey(void);


#endif /* KEYPAD_H_ */
