/*
 * hmi_ecu.c
 *
 *  Created on: Nov 1, 2024
 *      Author: Malak
 */

#include <avr/io.h>
#include "keypad.h"
#include "lcd.h"
#include "uart.h"
#include "timer.h"
#include "util/delay.h"

#define HMI_READY 	 	  0x10
#define CONTROL_READY 	  0x11
#define HMI_NEWPASS  	  0x12
#define HMI_NEWPASS2 	  0x13
#define CONTROL_RECEIVED  0x14
#define HMI_CHECK_PASS    0x15
#define CONTROL_SUCCESS   0x16
#define CONTROL_FAILURE	  0x17
#define CONTROL_MATCHED	  0x18
#define CONTROL_UNMATCHED 0x19
#define CONTROL_TIMEOVER  0x20
#define CONTROL_NOPEOPLE  0x21
#define HMI_OPEN_DOOR	  0x22
#define HMI_CHECKPEOPLE   0x23
#define HMI_CLOSEDOOR	  0x24
#define HMI_WRONGPASS	  0x25
#define HMI_TIMEOVER	  0x26
#define NUMBER_OF_COMPARE_MTACHES_FOR_PASS 60

const UART_ConfigType uartConfig = {EIGHT_BIT, EVEN_PARITY, ONE_BIT, 9600};
const Timer_ConfigType timerConfig = {0, 7813, TIMER1, TIMER_0_1_F_CPU_1024, CTC};

uint8 g_createPassword = 0;
uint8 g_LCD_state = 0;
uint8 g_KEYPAD_state = 0;
uint8 i = 0;
uint8 timerFlag = 0;
unsigned char g_tick = 0;

void pass_callBack(void)
{
	g_tick++;
	if(g_tick == NUMBER_OF_COMPARE_MTACHES_FOR_PASS)
	{
		g_LCD_state = 2;
		timerFlag = 0;
		UART_sendByte(HMI_READY);
		while(UART_recieveByte() != CONTROL_READY){};
		UART_sendByte(HMI_TIMEOVER);
		Timer_deInit(TIMER1);
	}
}

void LCD_function(void)
{
	switch(g_LCD_state)
	{
	case 0:
		LCD_clearScreen();
		LCD_displayString("Plz enter pass:");
		LCD_moveCursor(1,0);
		break;
	case 1:
		LCD_clearScreen();
		LCD_displayString("Plz re-enter the");
		LCD_moveCursor(1,0);
		LCD_displayString("same pass: ");
		LCD_moveCursor(1,10);
		break;
	case 2:
		LCD_clearScreen();
		LCD_displayString("+ : OPEN DOOR");
		LCD_moveCursor(1,0);
		LCD_displayString("- : CHANGE PASS");
		break;
	case 3:
		LCD_clearScreen();
		LCD_displayString("Plz enter old");
		LCD_moveCursor(1,0);
		LCD_displayString("pass:");
		LCD_moveCursor(1,5);
		break;
	case 4:
		LCD_clearScreen();
		LCD_displayString("Door Unlocking");
		LCD_moveCursor(1,4);
		LCD_displayString("Please wait");
		break;
	case 5:
		LCD_clearScreen();
		LCD_displayString("Wait for people");
		LCD_moveCursor(1,4);
		LCD_displayString("to enter");
		break;
	case 6:
		LCD_clearScreen();
		LCD_displayString("Door locking");
		break;
	case 7:
		LCD_clearScreen();
		LCD_displayString("System locked");
		LCD_moveCursor(1,0);
		LCD_displayString("Wait for 1 min");
		break;
	}
}

void Keypad_function(uint16 *password)
{
	for(i=0; i<5; i++)
	{
		password[i] = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		_delay_ms(500);
	}
	while(KEYPAD_getPressedKey() != '=');
}

int main()
{
	uint16 password[5];
	uint16 rePassword[5];
	uint8 trial = 0;
	uint16 MC2_PASS_STATE;
	uint8 menuChoice = 0;

	/* Drivers initialization */
	LCD_init();
	UART_init(&uartConfig);

	SREG |= (1<<7);   /* Enable interrupts */

	LCD_displayString("Door Lock System");

	_delay_ms(1000);

	while(1)
	{
		while(g_LCD_state == 0)
		{
			LCD_function();
			Keypad_function(password);
			_delay_ms(100);
			g_LCD_state = 1;
		}
		while(g_LCD_state == 1)
		{
			LCD_function();
			Keypad_function(rePassword);
			_delay_ms(100);
			UART_sendByte(HMI_READY);
			while(UART_recieveByte() != CONTROL_READY){};
			UART_sendByte(HMI_NEWPASS);
			while(UART_recieveByte() != CONTROL_READY){};
			for (i=0; i<5; i++)
			{
				UART_sendByte(password[i]);
				while(UART_recieveByte() != CONTROL_RECEIVED){};
			}
			_delay_ms(100);
			UART_sendByte(HMI_READY);
			while(UART_recieveByte() != CONTROL_READY){};
			UART_sendByte(HMI_NEWPASS2);
			while(UART_recieveByte() != CONTROL_READY){};
			for (i=0; i<5; i++)
			{
				UART_sendByte(rePassword[i]);
				while(UART_recieveByte() != CONTROL_RECEIVED){};
			}

			while(UART_recieveByte() != CONTROL_READY){};
			UART_sendByte(HMI_READY);

			if(UART_recieveByte() == CONTROL_MATCHED)
			{
				g_LCD_state = 2;
			}
			else
			{
				g_LCD_state = 0;
			}
		}
		while(g_LCD_state == 2)
		{
			menuChoice = 0;
			LCD_function();
			if(KEYPAD_getPressedKey() == '+')
			{
				_delay_ms(100);
				menuChoice = 1;
			}
			else if(KEYPAD_getPressedKey() == '-')
			{
				_delay_ms(100);
				menuChoice = 2;
			}
			g_LCD_state = 3;
		}
		while(g_LCD_state == 3)
		{
			LCD_function();
			trial = 0;
			while((trial<3) && (MC2_PASS_STATE != CONTROL_SUCCESS))
			{
				trial++;
				LCD_function();
				LCD_moveCursor(1,5);
				Keypad_function(password);
				_delay_ms(10);
				UART_sendByte(HMI_READY);
				while(UART_recieveByte() != CONTROL_READY){};
				UART_sendByte(HMI_CHECK_PASS);
				while(UART_recieveByte() != CONTROL_READY){};
				for (i=0; i<5; i++)
				{
					UART_sendByte(password[i]);
					while(UART_recieveByte() != CONTROL_RECEIVED){};
				}
				_delay_ms(10);

				//RETURN if the pass entered is the correct pass or not
				UART_sendByte(HMI_READY);
				while(UART_recieveByte() != CONTROL_READY){};
				MC2_PASS_STATE = UART_recieveByte();
			}

			if(trial == 3)
			{
				g_LCD_state = 7;
				trial = 0;
				break;
			}
			else if(MC2_PASS_STATE == CONTROL_SUCCESS)
			{
				if(menuChoice == 1)
				{
					g_LCD_state = 4;
				}
				else if (menuChoice == 2)
				{
					g_LCD_state = 0;
				}
			}
			MC2_PASS_STATE = 0;
		}
		while(g_LCD_state == 4)
		{
			LCD_function();
			//wait for UART2
			UART_sendByte(HMI_READY);
			while(UART_recieveByte() != CONTROL_READY){};
			UART_sendByte(HMI_OPEN_DOOR);
			while(UART_recieveByte() != CONTROL_TIMEOVER){};
			g_LCD_state = 5;
		}
		while(g_LCD_state == 5)
		{
			LCD_function();
			//wait for UART2 to say that PIR is not detecting
			UART_sendByte(HMI_READY);
			while(UART_recieveByte() != CONTROL_READY){};
			UART_sendByte(HMI_CHECKPEOPLE);
			while(UART_recieveByte() != CONTROL_NOPEOPLE){};
			g_LCD_state = 6;
		}
		while(g_LCD_state == 6)
		{
			LCD_function();
			//wait for UART2 to tell timer finished
			UART_sendByte(HMI_READY);
			while(UART_recieveByte() != CONTROL_READY){};
			UART_sendByte(HMI_CLOSEDOOR);
			while(UART_recieveByte() != CONTROL_TIMEOVER){};
			g_LCD_state = 2;
		}
		if((g_LCD_state == 7) && (timerFlag == 0))
		{
			timerFlag = 1;
			LCD_function();
			Timer_setCallBack(&pass_callBack, TIMER1);
			Timer_init(&timerConfig);
			UART_sendByte(HMI_READY);
			while(UART_recieveByte() != CONTROL_READY){};
			UART_sendByte(HMI_WRONGPASS);
		}
	}
}
