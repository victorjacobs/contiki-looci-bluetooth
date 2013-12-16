/*
 * uart.c
 */

#include "debug.h"
#include "bluetooth.h"
#include <avr/io.h>

// Serial helper functions
// Setup
void setupUART(int ubrr) {
	// Configure UART1 (digital pins 0 and 1)
	UCSR1A &= ~_BV(U2X1);	// Disable double rate
	UBRR1H = (unsigned char) (ubrr >> 8);
	UBRR1L = (unsigned char) ubrr;
	UCSR1B = _BV(RXEN1) | _BV(TXEN1);
}

void shutdownUART() {
	UCSR1A = 0;
	UCSR1B = 0;
	UBRR1H = 0;
	UBRR1L = 0;
}

// RX
bool receiveCompleted() {
	return (UCSR1A & _BV(RXC1)) != 0;
}

void serialReadString(char* buf) {
	int nlSeen = 0, crSeen = 0;
	char a;
	int i;
	
	do {
		nlSeen = 0;
		crSeen = 0;
		i = 0;
		
		while (!(nlSeen && crSeen)) {
			while (!receiveCompleted());
		
			a = UDR1;
			if (a == 0x0d) {
				crSeen = 1;
			} else {
				if (a == 0x0a) {
					nlSeen = 1;
					
				} else {
					buf[i] = a;
					i++;
				}
			}
		}
		
		// Null terminate
		buf[i] = 0;
	} while (i == 0);
}

// TX
bool sendCompleted() {
	return (UCSR1A & _BV(UDRE1)) != 0;
}

void serialWriteString(char* s) {
	int i = 0;
	while (s[i] != 0) {
		while (!sendCompleted());
	
		UDR1 = s[i];
		
		i++;
	}
}
