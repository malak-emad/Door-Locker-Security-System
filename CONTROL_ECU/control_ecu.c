/*
 * control_ecu.c
 *
 *  Created on: Nov 4, 2024
 *      Author: Malak
 */
#include <stdio.h>
#include <avr/io.h>
#include "uart.h"
#include "external_eeprom.h"
#include "twi.h"
#include "timer.h"
#include "dc_motor.h"
#include "pir.h"
#include "buzzer.h"
#include <util/delay.h>

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

#define NUMBER_OF_COMPARE_MTACHES_FOR_DOOR 15

/* global variable contain the ticks count of the timer */
unsigned char g_tick = 0;
unsigned char i = 0;

const UART_ConfigType uartConfig = {EIGHT_BIT, EVEN_PARITY, ONE_BIT, 9600};
const Timer_ConfigType timerConfig = {0, 7813, TIMER1, TIMER_0_1_F_CPU_1024, CTC};
TWI_ConfigType twiConfig = {0b00000010,4};

void door_callBack(void)
{
	g_tick++;
	if(g_tick == NUMBER_OF_COMPARE_MTACHES_FOR_DOOR)
	{
		DcMotor_Rotate(STOP, 0);
		g_tick = 0;
		Timer_deInit(TIMER1);
		UART_sendByte(CONTROL_TIMEOVER);
	}
}


uint8 UART_compareTwoPass(uint16 * password_one, uint16 *password_two)
{
	for(i=0; i<5 ;i++)
	{
		if (password_one[i] != password_two[i])
		{
			return FALSE;
			break;
		}
	}
	return TRUE;
}

int main()
{
	uint16 password[5];
	uint16 rePassword[5];
	uint16 CHECK[5];
	uint8 val = 0;

	DcMotor_init();
	PIR_init();
	BUZZER_init();
	TWI_init(&twiConfig);
	UART_init(&uartConfig);

	SREG |= (1<<7);   /* Enable interrupts */

	while(UART_recieveByte() != HMI_READY);
	UART_sendByte(CONTROL_READY);

	while(1)
	{
		uint16 UART_func = UART_recieveByte();
		switch(UART_func)
		{
		case HMI_READY:
			UART_sendByte(CONTROL_READY);
			break;

		case HMI_NEWPASS:
			UART_sendByte(CONTROL_READY);
			for(i=0; i<5; i++)
			{
				password[i] = UART_recieveByte();
				UART_sendByte(CONTROL_RECEIVED);
			}
			break;

		case HMI_NEWPASS2:
			UART_sendByte(CONTROL_READY);
			for(i=0; i<5; i++)
			{
				rePassword[i] = UART_recieveByte();
				UART_sendByte(CONTROL_RECEIVED);
			}
			_delay_ms(100);
			UART_sendByte(CONTROL_READY);
			while(UART_recieveByte() != HMI_READY){};
			if(UART_compareTwoPass(password, rePassword))
			{
				UART_sendByte(CONTROL_MATCHED);
				for(i=0; i<5; i++)
				{
					EEPROM_writeByte(0x0311 + i, password[i]);
					_delay_ms(10);
				}
				_delay_ms(10);

			}
			else UART_sendByte(CONTROL_UNMATCHED);
			break;

		case HMI_CHECK_PASS:
			UART_sendByte(CONTROL_READY);
			/* receive password from user */
			for(i=0; i<5; i++)
			{
				password[i] = UART_recieveByte();
				UART_sendByte(CONTROL_RECEIVED);
			}

			/* retrieve password from EEPROM */
			for(i=0; i<5; i++)
			{
				EEPROM_readByte(0x0311 + i, &val);
				CHECK[i]=val;
				_delay_ms(10);
			}

			while(UART_recieveByte() != HMI_READY){};
			UART_sendByte(CONTROL_READY);
			if(UART_compareTwoPass(password, CHECK))
			{
				UART_sendByte(CONTROL_SUCCESS);
			}
			else
			{
				UART_sendByte(CONTROL_FAILURE);
			}
			break;

		case HMI_OPEN_DOOR:
			DcMotor_Rotate(CLOCKWISE, 100);
			Timer_setCallBack(&door_callBack, TIMER1);
			Timer_init(&timerConfig);
			break;

		case HMI_CHECKPEOPLE:
			while(PIR_getState() == PIR_MOTION_DETECTED){};
			UART_sendByte(CONTROL_NOPEOPLE);
			break;

		case HMI_CLOSEDOOR:
			DcMotor_Rotate(ANTI_CLOCKWISE, 100);
			Timer_setCallBack(&door_callBack, TIMER1);
			Timer_init(&timerConfig);
			break;

		case HMI_WRONGPASS:
			BUZZER_on();
			break;

		case HMI_TIMEOVER:
			BUZZER_off();
			break;
		}
	}
}

