/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping.h"

volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile enum CLOCK_STATE STATE = LOW; // State of ping echo pulse
volatile uint16_t CLOCK_TIMEOUT_COUNT = 0;

void ping_init (void){

    //Enabling clock for Port B
    SYSCTL_RCGCGPIO_R |= 0x02;

    //Enable clock Timer 3
    SYSCTL_RCGCTIMER_R  |= 0x8;

    //Busy wait for registers
    while ((SYSCTL_PRGPIO_R & 0x02) == 0) {};
    while ((SYSCTL_PRTIMER_R & 0x8) == 0) {};

    //Enabling functionality on port b pins
    GPIO_PORTB_DEN_R |= GPIO_PORTB_PIN3_M;

    //Enabling alternate functions
    GPIO_PORTB_AFSEL_R |= GPIO_PORTB_PIN3_M;

    //setting pin as alternate function to timer3
    GPIO_PORTB_PCTL_R |= 0x7000;

    //shutting off control register while setting up
    TIMER3_CTL_R &= ~(0x100);

    //setting it to capture mode?

    TIMER3_TBMR_R |= 0x7;
    TIMER3_TBMR_R &= ~(0x10);

    TIMER3_CTL_R |= 0xC00;

    TIMER3_CFG_R |= 0x4;


    TIMER3_ICR_R |= (TIMERB_CAPTURE_EVENT_M|TIMERB_TIMEOUT_EVENT_M);

    TIMER3_IMR_R |= (TIMERB_CAPTURE_EVENT_M|TIMERB_TIMEOUT_EVENT_M);

    TIMER3_TBMATCHR_R = 0;
    TIMER3_TBPR_R |= TIMERB_PRESCALE_MAX_LOAD;
    TIMER3_TBILR_R |= TIMERB_INTERVAL_LOAD_MAX_LOAD;

    NVIC_EN1_R |= 0x10;

    TIMER3_CTL_R |= 0x100;

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

}

void ping_trigger (void){
    STATE = LOW;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~(0x100);
    TIMER3_IMR_R &= ~(TIMERB_CAPTURE_EVENT_M|TIMERB_TIMEOUT_EVENT_M);

    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R = ~(GPIO_PORTB_PIN3_M);

    GPIO_PORTB_DIR_R |= GPIO_PORTB_PIN3_M;

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE
    GPIO_PORTB_DATA_R &= ~(GPIO_PORTB_PIN3_M);
    GPIO_PORTB_DATA_R |= GPIO_PORTB_PIN3_M;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= ~(GPIO_PORTB_PIN3_M);

    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= (TIMERB_CAPTURE_EVENT_M|TIMERB_TIMEOUT_EVENT_M);
    while((TIMER3_RIS_R & TIMERB_CAPTURE_EVENT_M) == TIMERB_CAPTURE_EVENT_M && (TIMER3_RIS_R & TIMERB_TIMEOUT_EVENT_M) == TIMERB_TIMEOUT_EVENT_M);

    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0x8;
    TIMER3_IMR_R |= (TIMERB_CAPTURE_EVENT_M|TIMERB_TIMEOUT_EVENT_M);
    TIMER3_TBPR_R |= TIMERB_PRESCALE_MAX_LOAD;
    TIMER3_TBILR_R |= TIMERB_INTERVAL_LOAD_MAX_LOAD;
    TIMER3_CTL_R |= 0x100;

}

void TIMER3B_Handler(void){
  // YOUR CODE HERE
  // As needed, go back to review your interrupt handler code for the UART lab.
  // What are the first lines of code in the ISR? Regardless of the device, interrupt handling
  // includes checking the source of the interrupt and clearing the interrupt status bit.
  // Checking the source: test the MIS bit in the MIS register (is the ISR executing
  // because the input capture event happened and interrupts were enabled for that event?
  // Clearing the interrupt: set the ICR bit (so that same event doesn't trigger another interrupt)
  // The rest of the code in the ISR depends on actions needed when the event happens.


    if(TIMER3_MIS_R & TIMERB_CAPTURE_EVENT_M){
        TIMER3_ICR_R |= TIMERB_CAPTURE_EVENT_M;
        if(STATE == LOW){
            START_TIME = (TIMER3_TBR_R & TIMERB_READ_VALUE_M);
            STATE = HIGH;
            CLOCK_TIMEOUT_COUNT = 0;
            return;
        }else if(STATE == HIGH){
            END_TIME = (TIMER3_TBR_R & TIMERB_READ_VALUE_M);
            STATE = DONE;
            return;
        }
    }else if(TIMER3_MIS_R & TIMERB_TIMEOUT_EVENT_M){
        TIMER3_ICR_R |= TIMERB_TIMEOUT_EVENT_M;
        CLOCK_TIMEOUT_COUNT += 1;
        return;
    }

}

float ping_getDistance (void){
    // YOUR CODE HERE
    ping_trigger();
    while(STATE != DONE);

    unsigned long pulse_width = (START_TIME + pow(2, 24) * CLOCK_TIMEOUT_COUNT)-END_TIME;
    float distance = (pulse_width * CLOCK_CYCLE_LENGTH) * SPEED_OF_SOUND / 2;

    lcd_printf("CYCLES: %d\nDISTANCE: %.5lf\nBURST: %.5lf\nOVERFLOW: %d", pulse_width, distance, pulse_width * CLOCK_CYCLE_LENGTH, CLOCK_TIMEOUT_COUNT);
    START_TIME = END_TIME = CLOCK_TIMEOUT_COUNT = 0;

    STATE = LOW;

    return distance;
}
