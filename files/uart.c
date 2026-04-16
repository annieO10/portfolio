///*
//*
//*   uart.c
//*
//*
//*
//*   @author
//*   @date
//*/
//
//#include <inc/tm4c123gh6pm.h>
//#include <stdint.h>
//#include "uart.h"
//
//void uart_init(void){
//	//TODO
//  //enable clock to GPIO port B
//  SYSCTL_RCGCGPIO_R |= GPIO_PORTB_RCGC2_M;
//
//  //enable clock to UART1
//  SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_1_M;
//
//  //wait for GPIOB and UART1 peripherals to be ready
//  while ((SYSCTL_PRGPIO_R & GPIO_PORTB_RCGC2_M) == 0) {};
//  while ((SYSCTL_PRUART_R & SYSCTL_RCGCUART_1_M) == 0) {};
//
//  //enable alternate functions on port B pins
//  GPIO_PORTB_AFSEL_R |= GPIO_PORTB_AFSEL_M;
//
//  //enable digital functionality on port B pins
//  GPIO_PORTB_DEN_R |= GPIO_PORTB_DEN_M;
//
//  //enable UART1 Rx and Tx on port B pins
//  GPIO_PORTB_PCTL_R = GPIO_PORTB_PCTL_UART_M;
//
//  //calculate baud rate
//  uint16_t iBRD = 8; //use equations
//  uint16_t fBRD = 44; //use equations
//
//  //turn off UART1 while setting it up
//  UART1_CTL_R &= UART1_CTL_DISABLE_M;
//
//  //set baud rate
//  //note: to take effect, there must be a write to LCRH after these assignments
//  UART1_IBRD_R = iBRD;
//  UART1_FBRD_R = fBRD;
//
//  //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
//  //note: this write to LCRH must be after the BRD assignments
//  UART1_LCRH_R = UART1_LCRH_M;
//
//  //use system clock as source
//  //note from the datasheet UARTCCC register description:
//  //field is 0 (system clock) by default on reset
//  //Good to be explicit in your code
//  UART1_CC_R = 0x0;
//
//  //re-enable UART1 and also enable RX, TX (three bits)
//  //note from the datasheet UARTCTL register description:
//  //RX and TX are enabled by default on reset
//  //Good to be explicit in your code
//  //Be careful to not clear RX and TX enable bits
//  //(either preserve if already set or set them)
//  UART1_CTL_R = (UART1_CTL_ENABLE_M|UART1_CTL_ENABLE_TX_M|UART1_CTL_ENABLE_RX_M);
//
//}
//
//void uart_sendChar(char data){
//
//    while((UART1_FR_R & 0x20)!=0);
//
//    UART1_DR_R = data;
//
//    //TODO
//}
//
//char uart_receive(void){
//
//    uint32_t r;
//    char rData;
//
//    while((UART1_FR_R & 0x10) != 0);
//    r = UART1_DR_R;
//    if(r & 0xF00){
//        GPIO_PORTF_DATA_R = 0xF;
//    }
//    else{
//        rData = (char)UART1_DR_R & 0xFF;
//    }
//    return rData;
//}
//
//void uart_sendStr(const char *data){
//	if(strlen(0) <= 0){
//	    return;
//	}
//	int i = 0;
//	for(i = 0; i < strlen(data); ++i){
//	    uart_sendChar(data[i]);
//	}
//}
//
//char uart_recieve_nonblocking(){
//    uint32_t r;
//    char rData;
//
//    if((UART1_FR_R & 0x10) == 0){
//        return (char)0;
//    }
//
//    r = UART1_DR_R;
//
//    if(r & 0xF00){
//        GPIO_PORTF_DATA_R = 0xF;
//    }
//    else{
//        rData = (char)UART1_DR_R & 0xFF;
//    }
//    return rData;
//}
