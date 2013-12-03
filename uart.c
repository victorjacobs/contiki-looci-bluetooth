/*
 * uart.c
 */

#include "debug.h"
#include "bluetooth.h"
#include <avr/io.h>

// Serial helper functions
// Setup
void setupUART(int ubrr) {
	PRINTF("Setting up UART1 with UBRR %d\n", ubrr);
	// Configure UART1 (digital pins 0 and 1)
	UBRR1H = (unsigned char) (ubrr >> 8);
	UBRR1L = (unsigned char) ubrr;
	// Set 8 bit mode, asynchronous
	//UCSR1C = (1 << URSEL) | (3 << UCSZ0);
	UCSR1C = 3 << UCSZ01;
	// Enable RX and TX
	UCSR1B = (1 << RXEN1) | (1 << TXEN1);
}

// RX
bool receiveCompleted() {
	return (UCSR1A & _BV(RXC1)) != 0;
}

unsigned char serialReadByte() {
	while (!receiveCompleted());

	return UDR1;
}

char* serialReadString() {
	char ret[128];	// Don't allocate too much room for string
	char a;
	char* r;
	
	for (r = ret; (a = serialReadByte()) != 0 && r < (ret + 128); r++) {
		*r = a;
	}
	
	// Null terminate
	*(++r) = 0;
	
	return ret;
}

// TX
bool sendCompleted() {
	return (UCSR1A & _BV(UDRE1)) != 0;
}

void serialWriteByte(unsigned char DataOut) {
	while (!sendCompleted());

	UDR1 = DataOut;
}

void serialWriteString(char* s) {
	PRINTF("Writing %s to UART\n", s);
	while (*s != 0) {
		serialWriteByte(*s);
		s++;
	}
	
	// Write null byte
	serialWriteByte(0);
}
