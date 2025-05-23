/*
 * external_eeprom.c
 *
 *  Created on: Nov 2, 2024
 *      Author: Malak
 */

#include "external_eeprom.h"
#include "gpio.h"
#include "twi.h"


uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data)
{
	/* Send the Start Bit */
	TWI_start();
	if(!(TWI_getStatus() == TWI_START))
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8)((u16addr & 0x0700)>>7) | (0xA0));
	if(!(TWI_getStatus() == TWI_MT_SLA_W_ACK))
		return ERROR;

	/* Send the memory address */
	TWI_writeByte((uint8)(u16addr));
	if(!(TWI_getStatus() == TWI_MT_DATA_ACK))
		return ERROR;

	/* write byte to eeprom */
	TWI_writeByte(u8data);
	if(!(TWI_getStatus() == TWI_MR_DATA_ACK)){};

    /* Send the Stop Bit */
    TWI_stop();

    return SUCCESS;
}

uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data)
{
	/* Send the Start Bit */
	TWI_start();
	if(!(TWI_getStatus() == TWI_START))
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8)((u16addr & 0x0700)>>7) | (0xA0));
	if(!(TWI_getStatus() == TWI_MT_SLA_W_ACK))
		return ERROR;

	/* Send the memory address */
	TWI_writeByte((uint8)(u16addr));
	if(!(TWI_getStatus() == TWI_MT_DATA_ACK))
		return ERROR;

	/* Resend the Start Bit */
	TWI_start();
	if(!(TWI_getStatus() == TWI_REP_START))
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=1 (read) */
	TWI_writeByte((uint8)((u16addr & 0x0700)>>7) | (0xA0) | 1);
	if(!(TWI_getStatus() == TWI_MT_SLA_R_ACK))
		return ERROR;

	/* Read the data From EEPROM */
	*u8data = TWI_readByteWithNACK();
	if(!(TWI_getStatus() == TWI_MR_DATA_NACK))
		return ERROR;

    /* Send the Stop Bit */
    TWI_stop();

    return SUCCESS;
}
