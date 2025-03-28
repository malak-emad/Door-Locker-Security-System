/*
 * uart.h
 *
 *  Created on: Nov 1, 2024
 *      Author: Malak
 */

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
#define UART_BaudRateType uint32


typedef enum {
	FIVE_BIT, SIX_BIT, SEVEN_BIT, EIGHT_BIT, NINE_BIT = 7
}UART_BitDataType;

typedef enum {
	DIABLED, RESERVED, EVEN_PARITY, ODD_PARITY
}UART_ParityType;

typedef enum {
	ONE_BIT, TWO_BITS
}UART_StopBitType;

typedef struct {
	UART_BitDataType bit_data;
	UART_ParityType parity;
	UART_StopBitType stop_bit;
	UART_BaudRateType baud_rate;
}UART_ConfigType;


/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * Config_Type);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #


#endif /* UART_H_ */
