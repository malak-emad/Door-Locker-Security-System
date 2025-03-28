/*
 * twi.c
 *
 *  Created on: Nov 1, 2024
 *      Author: Malak
 */
#include "twi.h"
#include "gpio.h"
#include "common_macros.h"
#include <avr/io.h>

void TWI_init(const TWI_ConfigType * Config_Ptr)
{
	/* Assume TWPS1 and TWPS0 = 00 */
	TWSR = 0x00;

    /* Bit Rate: 400.000 kbps using zero pre-scaler TWPS=00 and F_CPU=8Mhz */
    TWBR = ((F_CPU/Config_Ptr->bit_rate)-16)/8;


    /* Two Wire Bus address my address if any master device want to call me: 0x1 (used in case this MC is a slave device)
       General Call Recognition: Off */
    TWAR = (Config_Ptr->address << 1); // my address = 0x01 :)  //CHECK!!!!!!

    TWCR = (1<<TWEN); /* enable TWI */
}

void TWI_start(void)
{
	/************************** TWCR Description **************************
	 * TWINT = 1 Clear The interrupt flag before sending the start bit
	 * TWEA  = 0 Disable Acknowledge Bit
	 * TWSTA = 1 Start Condition Bit
	 * TWSTO = 0 Disable Stop Condition Bit
	 * TWWC  = 0 No Collision
	 * TWEN  = 1 Enable TWI
	 * TWIE  = 0 Disable TWI Interrupt
	 ***********************************************************************/
	TWCR = (1<<TWEN) | (1<<TWSTA) | (1<<TWINT);

    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
	while(BIT_IS_CLEAR(TWCR, TWINT));

}
void TWI_stop(void)
{
	/************************** TWCR Description **************************
	 * TWINT = 1 Clear The interrupt flag before sending the start bit
	 * TWEA  = 0 Disable Acknowledge Bit
	 * TWSTA = 0 Start Condition Bit
	 * TWSTO = 1 Stop Condition Bit
	 * TWWC  = 0 No Collision
	 * TWEN  = 1 Enable TWI
	 * TWIE  = 0 Disable TWI Interrupt
	 ***********************************************************************/
	TWCR = (1<<TWEN) | (1<<TWSTO) | (1<<TWINT);
}

void TWI_writeByte(uint8 data)
{
	/************************** TWCR Description **************************
	 * TWINT = 1 Clear The interrupt flag before sending the start bit
	 * TWEA  = 0 Disable Acknowledge Bit
	 * TWSTA = 0 Disable Start Condition Bit
	 * TWSTO = 0 Disable Stop Condition Bit
	 * TWWC  = 0 No Collision
	 * TWEN  = 1 Enable TWI
	 * TWIE  = 0 Disable TWI Interrupt
	 ***********************************************************************/
	/* TWINT flag need to be SET to enable writing */
	TWDR = data;

	TWCR = (1<<TWEN) | (1<<TWINT);

	while(BIT_IS_CLEAR(TWCR, TWINT));

}

uint8 TWI_readByteWithACK(void)
{
	/************************** TWCR Description **************************
	 * TWINT = 1 Clear The interrupt flag before sending the start bit
	 * TWEA  = 1 Enable Acknowledge Bit
	 * TWSTA = 0 Disable Start Condition Bit
	 * TWSTO = 0 Disable Stop Condition Bit
	 * TWWC  = 0 No Collision
	 * TWEN  = 1 Enable TWI
	 * TWIE  = 0 Disable TWI Interrupt
	 ***********************************************************************/
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
	return TWDR;
}

uint8 TWI_readByteWithNACK(void)
{
	/************************** TWCR Description **************************
	 * TWINT = 1 Clear The interrupt flag before sending the start bit
	 * TWEA  = 0 Disable Acknowledge Bit
	 * TWSTA = 0 Disable Start Condition Bit
	 * TWSTO = 0 Disable Stop Condition Bit
	 * TWWC  = 0 No Collision
	 * TWEN  = 1 Enable TWI
	 * TWIE  = 0 Disable TWI Interrupt
	 ***********************************************************************/
	TWCR = (1<<TWEN) | (1<<TWINT);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
	return TWDR;
}

uint8 TWI_getStatus(void)
{
	 /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
	return (TWSR & 0xF8);
}

