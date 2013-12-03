/*
 * uart.h
 */

#ifndef UART_H_
#define UART_H_

// Serial helper functions
void setupUART(int baud);

bool receiveCompleted(void);
unsigned char serialReadByte(void);
char* serialReadString(void);

bool sendCompleted(void);
void serialWriteByte(unsigned char DataOut);
void serialWriteString(char* s);


#endif /* UART_H_ */
