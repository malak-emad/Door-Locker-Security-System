/*
 * uart.c
 *
 *  Created on: Nov 1, 2024
 *      Author: Malak
 */
#include "uart.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "gpio.h"
#include <avr/io.h>


/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * Config_Type)
{
	uint16 ubrr_value = 0;

	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);

	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART TX Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/
	UCSRB = (1<<RXEN) | (1<<TXEN);
	if( Config_Type->bit_data == NINE_BIT)
		UCSRB = (1<<UCSZ2);


	/************************** UCSRC Description **************************
	 * URSEL = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL = 0 Asynchronous Operation
	 * UPM1:0 Based on the parity chose
	 * USBS   Based on number of stop bits
	 * UCSZ1:0 Based on data mode
	 * UCPOL = 0 Used with the Synchronous operation only
	 **********************************************************************/
	UCSRC = (1<<URSEL);
	UCSRC = (UCSRC & 0xCF) | (Config_Type->parity << 4);   /* Parity */
	SET_BIT(UCSRC,Config_Type->stop_bit);
	UCSRC = (UCSRC & 0xF9) | (Config_Type->bit_data << 1);   /* Data Bit mode */


	/* Calculate the UBRR register value */
	ubrr_value = (uint16)(((F_CPU / (Config_Type->baud_rate * 8UL))) - 1);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;
}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data)
{
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR = data;
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA, RXC));
	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
	return UDR;
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str)
{
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str) // Receive until #
{
	while(*Str != '#')
	{
		*Str = UART_recieveByte();
		Str++;
	}
	*Str = '\0';
}



