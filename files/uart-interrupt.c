/*
*
*   uart-interrupt.c
*
*
*
*   @author
*   @date
*/

// The "???" placeholders should be the same as in your uart.c file.
// The "?????" placeholders are new in this file and must be replaced.

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "uart-interrupt.h"

// These variables are declared as examples for your use in the interrupt handler.
volatile char command_byte = 0; // byte value for special character used as a command
volatile int command_flag = 0; // flag to tell the main program a special command was received

volatile float OBJECT_CUTOFF_TOLERANCE = 0.15;
volatile float OBJECT_DIFFERENCE_TOLERANCE = 0.80;
volatile uint8_t OBJECT_RADIAL_WIDTH_CUTOFF = 4;

void uart_interrupt_init(void){
	//TODO
  //enable clock to GPIO port B
  SYSCTL_RCGCGPIO_R |= GPIO_PORTB_RCGC2_M;

  //enable clock to UART1
  SYSCTL_RCGCUART_R |= UART_RCGCUART;

  //wait for GPIOB and UART1 peripherals to be ready
  while ((SYSCTL_PRGPIO_R & GPIO_PORTB_RCGC2_M) == 0) {};
  while ((SYSCTL_PRUART_R & UART_RCGCUART) == 0) {};

  //enable digital functionality on port B pins
  GPIO_PORTB_DEN_R |= GPIO_PORTB_DEN_M;

  //enable alternate functions on port B pins
  GPIO_PORTB_AFSEL_R |= GPIO_PORTB_AFSEL_M;

  //enable UART1 Rx and Tx on port B pins
  GPIO_PORTB_PCTL_R = GPIO_PORTB_PCTL_UART_M;

  //calculate baud rate
  uint16_t iBRD = 8; //use equations
  uint16_t fBRD = 44; //use equations

  //turn off UART1 while setting it up
  UART1_CTL_R &= UART1_CTL_DISABLE_M;

  //set baud rate
  //note: to take effect, there must be a write to LCRH after these assignments
  UART1_IBRD_R = iBRD;
  UART1_FBRD_R = fBRD;

  //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
  //note: this write to LCRH must be after the BRD assignments
  UART1_LCRH_R = UART1_LCRH_M;

  //use system clock as source
  //note from the datasheet UARTCC register description:
  //field is 0 (system clock) by default on reset
  //Good to be explicit in your code
  UART1_CC_R = 0x0;

  //////Enable interrupts

  //first clear RX interrupt flag (clear by writing 1 to ICR)
  UART1_ICR_R |= 0b00010000;

  //enable RX raw interrupts in interrupt mask register
  UART1_IM_R |= 0x10;

  //NVIC setup: set priority of UART1 interrupt to 1 in bits 21-23
  NVIC_PRI1_R = (NVIC_PRI1_R & 0xFF0FFFFF) | 0x00200000;

  //NVIC setup: enable interrupt for UART1, IRQ #6, set bit 6
  NVIC_EN0_R |= 0x40;

  //tell CPU to use ISR handler for UART1 (see interrupt.h file)
  //from system header file: #define INT_UART1 22
  //Init in startup_css.c
  IntRegister(INT_UART1, UART1_Handler);

  //globally allow CPU to service interrupts (see interrupt.h file)
  IntMasterEnable();

  //re-enable UART1 and also enable RX, TX (three bits)
  //note from the datasheet UARTCTL register description:
  //RX and TX are enabled by default on reset
  //Good to be explicit in your code
  //Be careful to not clear RX and TX enable bits
  //(either preserve if already set or set them)
  UART1_CTL_R = (UART1_CTL_ENABLE_M|UART1_CTL_ENABLE_TX_M|UART1_CTL_ENABLE_RX_M);

}

void uart_sendChar(char data){

    while((UART1_FR_R & 0x20)!=0);

        UART1_DR_R = data;
}

char uart_receive(void){
	//DO NOT USE this busy-wait function if using RX interrupt
    if((UART1_IM_R & 0x10) == 0){
        uint32_t r;
        char rData;

        while((UART1_FR_R & 0x10) != 0);
        r = UART1_DR_R;
        if(r & 0xF00){
            GPIO_PORTF_DATA_R = 0xF;
        }
        else{
            rData = (char)UART1_DR_R & 0xFF;
        }
        return rData;
    }
    return 0;
}

void uart_sendStr(const char *data){

    if(strlen(0) <= 0){
            return;
        }
        int i = 0;
        for(i = 0; i < strlen(data); ++i){
            uart_sendChar(data[i]);
        }
}

// Interrupt handler for receive interrupts
void UART1_Handler(void)
{
    char byte_received;
    //check if handler called due to RX event
    if (UART1_MIS_R & 0x10)
    {
        //byte was received in the UART data register
        //clear the RX trigger flag (clear by writing 1 to ICR)
        UART1_ICR_R |= 0b00010000;

        //read the byte received from UART1_DR_R and echo it back to PuTTY
        //ignore the error bits in UART1_DR_R
        byte_received = UART1_DR_R;

//        uart_sendChar(byte_received);

        //if byte received is a carriage return
        if (byte_received == '\r')
        {
            //send a newline character back to PuTTY
            uart_sendChar('\n');
        }
        else
        {
            //AS NEEDED
            //code to handle any other special characters
            //code to update global shared variables
            //DO NOT PUT TIME-CONSUMING CODE IN AN ISR

            if(command_flag == 6 | command_flag == 7 | command_flag == 8 | command_flag == 9){
                command_byte = byte_received;
                return;
            }

            if (byte_received == '2'){
                command_flag = 2;
            }else if(byte_received == '3'){
                command_flag = 3;
            }else if(byte_received == '4'){
                command_flag = 4;
            }else if(byte_received == '5'){
                command_flag = 5;
            }else if(byte_received == '6'){
                command_flag = 6;
            }else if(byte_received == '7'){
                command_flag = 7;
            }else if(byte_received == '8'){
                command_flag = 8;
            }else if(byte_received == '9'){
                command_flag = 9;
            }else if(byte_received == ':'){
                command_flag = 10;
            }else if(byte_received == ';'){
                command_flag = 11;
            }else if(byte_received == '<'){
                command_flag = 12;
            }else if(byte_received == '='){
                command_flag = 13;
            }else if(byte_received == '>'){
                command_flag = 14;
            }else if(byte_received == '?'){
                command_flag = 15;
            }else if(byte_received == '@'){
                command_flag = 16;
            }else{
                command_byte = byte_received;
            }

        }
    }
}
