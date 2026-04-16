/*
*
*   uart.h
*
*   Used to set up the RS232 connector and WIFI module
*   Functions for communicating between CyBot and PC via UART1
*   Serial parameters: Baud = 115200, 8 data bits, 1 stop bit,
*   no parity, no flow control on COM1, FIFOs disabled on UART1
*
*   @author Dane Larson
*   @date 07/18/2016
*   Phillip Jones updated 9/2019, removed WiFi.h, Timer.h
*   Diane Rover updated 2/2020, changed receive return type to char
*/

#ifndef UART_H_
#define UART_H_

#include <inc/tm4c123gh6pm.h>
#include <string.h>

#ifndef GPIO_PORTB_RCGC2_M
#define GPIO_PORTB_RCGC2_M 0x02
#endif

#ifndef GPIO_PORTB_DEN_M
#define GPIO_PORTB_DEN_M 0x03
#endif

#ifndef GPIO_PORTB_DIR_M
#define GPIO_PORTB_DIR_M 0x03
#endif

#ifndef GPIO_PORTB_AFSEL_M
#define GPIO_PORTB_AFSEL_M 0x03
#endif

#ifndef GPIO_PORTB_PCTL_UART_M
#define GPIO_PORTB_PCTL_UART_M 0x00000011
#endif

#ifndef SYSCTL_RCGCUART_1_M
#define SYSCTL_RCGCUART_1_M 0x2
#endif

#ifndef UART1_CTL_DISABLE_M
#define UART1_CTL_DISABLE_M 0x0
#endif

#ifndef UART1_CTL_ENABLE_M
#define UART1_CTL_ENABLE_M 0x1
#endif

#ifndef UART1_CTL_ENABLE_TX_M
#define UART1_CTL_ENABLE_TX_M 0x100
#endif

#ifndef UART1_CTL_ENABLE_RX_M
#define UART1_CTL_ENABLE_RX_M 0x200
#endif

#ifndef UART1_LCRH_M
#define UART1_LCRH_M 0b01100000
#endif

// UART1 device initialization for CyBot to PuTTY
void uart_init(void);

// Send a byte over UART1 from CyBot to PuTTY
void uart_sendChar(char data);

// CyBot waits (i.e. blocks) to receive a byte from PuTTY
// returns byte that was received by UART1
char uart_receive(void);

// Send a string over UART1
// Sends each char in the string one at a time
void uart_sendStr(const char *data);

char uart_recieve_nonblocking();

#endif /* UART_H_ */
