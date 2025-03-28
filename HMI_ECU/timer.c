/*
 * timer.c
 *
 *  Created on: Nov 2, 2024
 *      Author: Malak
 */
#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
void (*g_TIMER0_callBackPtr)(void) = NULL_PTR;
void (*g_TIMER1_callBackPtr)(void) = NULL_PTR;
void (*g_TIMER2_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect){
	if(g_TIMER0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_TIMER0_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_COMP_vect){
	if(g_TIMER0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_TIMER0_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_OVF_vect){
	if(g_TIMER1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_TIMER1_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPA_vect){
	if(g_TIMER1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_TIMER1_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_OVF_vect){
	if(g_TIMER2_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_TIMER2_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_COMP_vect){
	if(g_TIMER2_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_TIMER2_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description: Function to initialize the Timer driver
   Inputs: pointer to the configuration structure with type Timer_ConfigType.
   Return: None
 */
void Timer_init(const Timer_ConfigType * Config_Ptr)
{
	if (Config_Ptr ->timer_ID == TIMER0)
	{
		/* configure  WGM01:0 */
		TCCR0 = (TCCR0 & 0xB7) | (((Config_Ptr ->timer_mode) & 0x02) <<2) | (((Config_Ptr ->timer_mode) & 0x01) <<7);
		/* Force Output Compare Match when PWM is disabled */
		TCCR0 = (TCCR0 & 0x7F) | (0x01 << 7);		/*FOC0*/

		if (Config_Ptr -> timer_mode == CTC)
		{
			OCR0 = Config_Ptr -> timer_compare_MatchValue;
			TIMSK |= (1<<OCIE0); // Enable Timer0 Compare Interrupt
		}
		else{
			TIMSK |= (1<<TOIE0);   /*enables the interrupt */
		}

		TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr -> timer_clock);   /* Set Prescaler */

		TCNT0 = Config_Ptr -> timer_InitialValue;
	}
	else if (Config_Ptr ->timer_ID == TIMER1)
	{
		TCNT1 = Config_Ptr -> timer_InitialValue;

		/* WGM0:1 are always 0 for normal and compare mode */

		if(Config_Ptr -> timer_mode == CTC)
		{
			TCCR1A = (1<<FOC1A);		/* Force Output Compare match */
			TCCR1B = (1<<WGM12);
			OCR1A = Config_Ptr -> timer_compare_MatchValue;
			TIMSK |= (1<<OCIE1A);
		}
		else if(Config_Ptr -> timer_mode == NORMAL)
		{
			TCCR1A = (1<<FOC1A);		/* Force Output Compare match */
			TIMSK |= (1<<TOIE1);
		}
		TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr -> timer_clock);

	}
	else if (Config_Ptr -> timer_ID == TIMER2)
	{
		/* configure  WGM01:0 */
		TCCR2 = (TCCR2 & 0xB7) | (((Config_Ptr ->timer_mode) & 0x02) <<2) | (((Config_Ptr ->timer_mode) & 0x01) <<7);
		/* Force Output Compare Match when PWM is disabled */
		TCCR2 = (TCCR2 & 0x7F) | (0x01 << 7);		/*FOC2*/
		if (Config_Ptr -> timer_mode == CTC)
		{
			OCR2 = Config_Ptr -> timer_compare_MatchValue;
			TIMSK |= (1<<OCIE2); // Enable Timer0 Compare Interrupt
		}
		else{
			TIMSK |= (1<<TOIE2);   /*enables the interrupt */
		}

		TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr -> timer_clock);   /* Set Prescaler */

		TCNT2 = Config_Ptr -> timer_InitialValue;
	}
}

/*
 * Description: Function to disable the Timer via Timer_ID.
 * Inputs: Timer_ID
 *  Return: None
 */
void Timer_deInit(Timer_ID_Type timer_type)
{
	switch (timer_type)
	{
	case TIMER0:
		TCCR0 = 0;
		TCNT0 = 0;
		TIMSK &= ~(1<<OCIE0);
		TIMSK &= ~(1<<TOIE0);
		break;

	case TIMER1:
		TCCR1A = 0;
		TCCR1B = 0;
		TCNT1 = 0;
		TIMSK &= ~(1<<OCIE1A);
		TIMSK &= ~(1<<TOIE1);
		break;

	case TIMER2:
		TCCR2 = 0;
		TCNT2 = 0;
		TIMSK &= ~(1<<OCIE2);
		TIMSK &= ~(1<<TOIE2);
		break;
	}
}


/*
 * Description: Function to set the Call Back function address to the required Timer.
 * Inputs: pointer to Call Back function and Timer Id you want to set The Callback to it.
 * Return: None
 */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID )
{
	/* Save the address of the Call back function in a global variable */
	switch (a_timer_ID) {
	case TIMER0:
		g_TIMER0_callBackPtr = a_ptr;
		break;
	case TIMER1:
		g_TIMER1_callBackPtr = a_ptr;
		break;
	case TIMER2:
		g_TIMER2_callBackPtr = a_ptr;
		break;
	}
}
