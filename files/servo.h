/*
 * servo.h
 *
 *  Created on: Apr 9, 2024
 *      Author: nperk03
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include "lcd.h"

#ifndef PWM_PERIOD_LENGTH
#define PWM_PERIOD_LENGTH 0x4E200
#endif

#ifndef GPIO_PORTB_PIN5_M
#define GPIO_PORTB_PIN5_M 0x20
#endif

#ifndef TIMERB_CAPTURE_EVENT_M
#define TIMERB_CAPTURE_EVENT_M 0x400
#endif

#ifndef TIMERB_TIMEOUT_EVENT_M
#define TIMERB_TIMEOUT_EVENT_M 0x100
#endif

#ifndef TIMERB_MATCH_EVENT_M
#define TIMERB_MATCH_EVENT_M 0x800
#endif

#ifndef TIMERB_PRESCALE_MAX_LOAD
#define TIMERB_PRESCALE_MAX_LOAD 0xFF
#endif

#ifndef TIMERB_INTERVAL_LOAD_MAX_LOAD
#define TIMERB_INTERVAL_LOAD_MAX_LOAD 0xFFFF
#endif

#ifndef CLOCK_CYCLE_LENGTH_NS
#define CLOCK_CYCLE_LENGTH_NS 62.5
#endif

#ifdef MAX_CLOCK_COUNT
#define MAX_CLOCK_COUNT 32000
#endif

#ifdef MIN_CLOCK_COUNT
#define MIN_CLOCK_COUNT 8000
#endif

enum SERVO_DIRECTION_E {CLOCKWISE, COUNTER_CLOCKWISE};
typedef enum SERVO_DIRECTION_E SERVO_DIRECTION_E;

void servo_init();

void servo_move(int16_t);

void set_servo_direction(SERVO_DIRECTION_E);

SERVO_DIRECTION_E get_servo_direction();

//Point Servo at 90 Degrees using buttons and then use current angle as parameter for this function
void set_servo_bias(int16_t);

int16_t get_servo_bias();

int32_t get_CLOCK_CYCLE_COUNT();

void TIMER1B_Handler();

#endif /* SERVO_H_ */
