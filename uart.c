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
	UCSR1A &= ~_BV(U2X1);
	UBRR1H = (unsigned char) (ubrr >> 8);
	UBRR1L = (unsigned char) ubrr;
	UCSR1B = _BV(RXEN1) | _BV(TXEN1);
}

// RX
bool receiveCompleted() {
	return (UCSR1A & _BV(RXC1)) != 0;
}

void serialReadString(char* buf) {
	int nlSeen = 0, crSeen = 0;
	char a;
	int i;
	
	PRINTF("read: ");
	
	for (i = 0; nlSeen != 2 && crSeen != 2; i++) {
		while (!receiveCompleted());
		
		a = UDR1;
		
		if (a == 0x0d) {
			PRINT_LN("cr seen");
			crSeen++;
		} else {
			if (a == 0x0a) {
				PRINT_LN("nl seen");
				nlSeen++;
				
			} else {
				buf[i] = a;
			}
		}
	}
	
	// Null terminate
	buf[i + 1] = 0;
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
