/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */
#ifndef PING_H_
#define PING_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "Timer.h"
#include "ping.h"
#include "lcd.h"
#include <math.h>

#ifndef TIMERB_CAPTURE_EVENT_M
#define TIMERB_CAPTURE_EVENT_M 0x400
#endif

#ifndef TIMERB_TIMEOUT_EVENT_M
#define TIMERB_TIMEOUT_EVENT_M 0x100
#endif

#ifndef GPIO_PORTB_PIN3_M
#define GPIO_PORTB_PIN3_M 0x8
#endif

#ifndef TIMERB_PRESCALE_MAX_LOAD
#define TIMERB_PRESCALE_MAX_LOAD 0xFF
#endif

#ifndef TIMERB_INTERVAL_LOAD_MAX_LOAD
#define TIMERB_INTERVAL_LOAD_MAX_LOAD 0xFFFF
#endif

#ifndef TIMERB_READ_VALUE_M
#define TIMERB_READ_VALUE_M 0xFFFFFF
#endif

#define CLOCK_CYCLE_LENGTH 0.0000000625

#define SPEED_OF_SOUND 343

enum CLOCK_STATE{LOW,HIGH,DONE};

extern volatile unsigned long START_TIME;
extern volatile unsigned long END_TIME;
extern volatile enum CLOCK_STATE STATE;
extern volatile uint16_t CLOCK_TIMEOUT_COUNT;

/**
 * Initialize ping sensor. Uses PB3 and Timer 3B
 */
void ping_init (void);

/**
 * @brief Trigger the ping sensor
 */
void ping_trigger (void);

/**
 * @brief Timer3B ping ISR
 */
void TIMER3B_Handler(void);

/**
 * @brief Calculate the distance in cm
 *
 * @return Distance in cm
 */
float ping_getDistance (void);

#endif /* PING_H_ */
