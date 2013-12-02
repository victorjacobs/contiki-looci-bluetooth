/*
 * uart.h
 */

#ifndef UART_H_
#define UART_H_

// Serial helper functions
bool receiveCompleted(void);

bool sendCompleted(void);

unsigned char serialReadByte(void);

char* serialReadString(void);

void serialWriteByte(unsigned char DataOut);

void serialWriteString(char* s);


#endif /* UART_H_ */
