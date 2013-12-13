/*
 * uart.h
 */

#ifndef UART_H_
#define UART_H_

// Serial helper functions
void setupUART(int baud);

bool receiveCompleted(void);
void serialReadString(char* buf);

bool sendCompleted(void);
void serialWriteString(char* s);


#endif /* UART_H_ */
