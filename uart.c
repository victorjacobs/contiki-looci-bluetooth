/*
 * uart.c
 */

#include "debug.h"
#include "bluetooth.h"
#include <avr/io.h>

// Serial helper functions
bool receiveCompleted() {
	return (UCSR1A & _BV(RXC1)) != 0;
}

bool sendCompleted() {
	return (UCSR1A & _BV(UDRE1)) != 0;
}

unsigned char serialReadByte() {
	while (!receiveCompleted());

	return UDR1;
}

void serialWriteByte(unsigned char DataOut) {
	while (!sendCompleted());

	UDR1 = DataOut;
}

void serialWriteString(char* s) {
	PRINTF("Writing %s to UART", s);
	while (*s != 0) {
		serialWriteByte(*s);
		s++;
	}
	
	// Write null byte
	serialWriteByte(0);
}
