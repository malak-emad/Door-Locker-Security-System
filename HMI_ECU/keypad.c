/*
 * keypad.c
 *
 *  Created on: Sep 27, 2024
 *      Author: Malak
 */
#include "keypad.h"
#include "gpio.h"
#include<util/delay.h>

/*******************************************************************************
 *                      Functions Prototypes(Private)                          *
 *******************************************************************************/

#if KEYPAD_COLUMNS_NUMBER == 3
	static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number);
#elif KEYPAD_COLUMNS_NUMBER == 4
	static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number);
#endif
/*******************************************************************************
*                      Functions Definitions                                  *
*******************************************************************************/
/*
 * Description :
 * Get the Keypad pressed button
 */
uint8 KEYPAD_getPressedKey(void){
	uint8 button_number, row, col;

	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_FIRST_PIN_ID, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_FIRST_PIN_ID+1, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_FIRST_PIN_ID+2, PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_FIRST_PIN_ID+3, PIN_INPUT);


	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID,KEYPAD_COL_FIRST_PIN_ID,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID,KEYPAD_COL_FIRST_PIN_ID+1,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID,KEYPAD_COL_FIRST_PIN_ID+2,PIN_INPUT);
#if KEYPAD_COLUMNS_NUMBER == 4
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID,KEYPAD_COL_FIRST_PIN_ID+3,PIN_INPUT);
#endif
	while(1){
		for(row=0; row<KEYPAD_ROWS_NUMBER; row++){
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_FIRST_PIN_ID+row, PIN_OUTPUT);
			GPIO_writePin(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_FIRST_PIN_ID+row, KEYPAD_BUTTON_PRESSED);

			for(col=0; col<KEYPAD_COLUMNS_NUMBER; col++){
				if(GPIO_readPin(KEYPAD_COL_PORT_ID, KEYPAD_COL_FIRST_PIN_ID+col) == KEYPAD_BUTTON_PRESSED){
					button_number = (row * KEYPAD_COLUMNS_NUMBER) + col + 1;

#if KEYPAD_COLUMNS_NUMBER == 3
					return KEYPAD_4x3_adjustKeyNumber(button_number);
#elif KEYPAD_COLUMNS_NUMBER == 4
					return KEYPAD_4x4_adjustKeyNumber(button_number);
#endif
				}
			}
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_ROW_FIRST_PIN_ID+row,PIN_INPUT);
			_delay_ms(5);
		}

	}
}

#if KEYPAD_COLUMNS_NUMBER == 3
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number){
	uint8 keypad_button = 0;
	switch(button_number)
	{
	case 10:
		keypad_button = '*';
		break;
	case 11:
		keypad_button = 0;
		break;
	case 12:
		keypad_button = '#';
		break;
	default:
		keypad_button = button_number;
		break;
	}
	return keypad_button;
}

#elif KEYPAD_COLUMNS_NUMBER == 4

static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number){
	uint8 keypad_button = 0;
	switch(button_number)
	{
	case 1:
		keypad_button = 7;
		break;
	case 2:
		keypad_button = 8;
		break;
	case 3:
		keypad_button = 9;
		break;
	case 4:
		keypad_button = '%';
		break;
	case 5:
		keypad_button = 4;
		break;
	case 6:
		keypad_button = 5;
		break;
	case 7:
		keypad_button = 6;
		break;
	case 8:
		keypad_button = 'x';
		break;
	case 9:
		keypad_button = 1;
		break;
	case 10:
		keypad_button = 2;
		break;
	case 11:
		keypad_button = 3;
		break;
	case 12:
		keypad_button = '-';
		break;
	case 13:
		keypad_button = 'c';
		break;
	case 14:
		keypad_button = 0;
		break;
	case 15:
		keypad_button = '=';
		break;
	case 16:
		keypad_button = '+';
		break;
	}
	return keypad_button;
}
#endif
