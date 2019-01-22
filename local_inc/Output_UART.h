/*
 * Output_UART.h
 *
 *  Created on: 16.01.2019
 *      Author: Christian
 */

#ifndef LOCAL_INC_OUTPUT_UART_H_
#define LOCAL_INC_OUTPUT_UART_H_

int output_UART_read(UART_Handle uart);

void output_send(char msg[]);

void setDebugMode(Bool _debugMode);

#endif /* LOCAL_INC_OUTPUT_UART_H_ */
