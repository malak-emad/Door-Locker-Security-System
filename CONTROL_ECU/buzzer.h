 /******************************************************************************
 * buzzer.h
 *
 *  Created on: Oct 3, 2024
 *      Author: Malak
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

/********************************************************************************
 *                     			 Definitions                                    *
 *******************************************************************************/
#define BUZZER_PORT_OUTPUT_ID			PORTC_ID
#define BUZZER_PIN_OUTPUT_ID			PIN7_ID

#define BUZZER_OFF						LOGIC_LOW
#define BUZZER_ON						LOGIC_HIGH

/********************************************************************************
 *                          Functions Prototypes                                *
 *******************************************************************************/
/*
 * Description :
 * Function responsible of initializing the Buzzer driver.
 */
void BUZZER_init(void);

/*
 * Description :
 * Function responsible of turning On the Buzzer.
 */
void BUZZER_on(void);

/*
 * Description :
 * Function responsible of turning Off the Buzzer.
 */
void BUZZER_off(void);

#endif /* BUZZER_H_ */
