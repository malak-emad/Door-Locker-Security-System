/*
 * timer.h
 *
 *  Created on: Nov 2, 2024
 *      Author: Malak
 */

#ifndef TIMER_H_
#define TIMER_H_

#define F_CPU 8000000UL

#include "gpio.h"

typedef enum
{
	NORMAL, PWM_PHASE_CORRECT, CTC, FAST_PWM
}Timer_ModeType;

typedef enum
{
	TIMER0, TIMER1, TIMER2
}Timer_ID_Type;

typedef enum
{
	/* Clock prescalers for Timer1 and Timer0 */
	TIMER_0_1_NO_CLOCK  = 0,
	TIMER_0_1_F_CPU 	= 1,
	TIMER_0_1_F_CPU_8	= 2,
	TIMER_0_1_F_CPU_64	= 3,
	TIMER_0_1_F_CPU_256	= 4,
	TIMER_0_1_F_CPU_1024= 5,

	/* Clock prescalers for Timer2 */
	TIMER_2_NO_CLOCK	= 0,
	TIMER_2_F_CPU 		= 1,
	TIMER_2_F_CPU_8		= 2,
	TIMER_2_F_CPU_32	= 3,
	TIMER_2_F_CPU_64	= 4,
	TIMER_2_F_CPU_128	= 5,
	TIMER_2_F_CPU_256	= 6,
	TIMER_2_F_CPU_1024	= 7

}Timer_ClockType;

typedef struct
{
	uint16 timer_InitialValue;
	uint16 timer_compare_MatchValue; /*it will be used in compare mode only*/
	Timer_ID_Type timer_ID;
	Timer_ClockType timer_clock;
	Timer_ModeType timer_mode;
}Timer_ConfigType;


/*
 * Description: Function to initialize the Timer driver
   Inputs: pointer to the configuration structure with type Timer_ConfigType.
   Return: None
 */
void Timer_init(const Timer_ConfigType * Config_Ptr);

/*
 * Description: Function to disable the Timer via Timer_ID.
 * Inputs: Timer_ID
 *  Return: None
 */
void Timer_deInit(Timer_ID_Type timer_type);


/*
 * Description: Function to set the Call Back function address to the required Timer.
 * Inputs: pointer to Call Back function and Timer Id you want to set The Callback to it.
 * Return: None
 */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID );



#endif /* TIMER_H_ */
