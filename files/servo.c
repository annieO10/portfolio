/*
 * servo.c
 *
 *  Created on: Apr 9, 2024
 *      Author: nperk03
 */

#include "servo.h"

volatile static SERVO_DIRECTION_E SERVO_DIRECTION = COUNTER_CLOCKWISE;
volatile static uint16_t PREVIOUS_ANGLE = 0;
volatile static uint32_t CLOCK_CYCLE_COUNT = 0;
volatile static int16_t SERVO_BIAS = 0;
volatile static int16_t  MAX_CLOCK_COUNT = 0;
volatile static int16_t  MIN_CLOCK_COUNT = 0;

void servo_init(){
    SYSCTL_RCGCGPIO_R |= 0x02;
    SYSCTL_RCGCTIMER_R  |= 0x2;

    while ((SYSCTL_PRGPIO_R & 0x02) == 0) {};
    while ((SYSCTL_PRTIMER_R & 0x2) == 0) {};

    GPIO_PORTB_DIR_R |= GPIO_PORTB_PIN5_M;
    GPIO_PORTB_DEN_R |= GPIO_PORTB_PIN5_M;
    GPIO_PORTB_AFSEL_R |= GPIO_PORTB_PIN5_M;
    GPIO_PORTB_PCTL_R |= 0x700000;

    TIMER1_CTL_R &= ~(0x100);
    TIMER1_CFG_R |= 0x4;
    TIMER1_TBMR_R |= 0x22A;

    TIMER1_CTL_R &= ~0xC00;

    TIMER1_ICR_R |= 0x200 | TIMERB_MATCH_EVENT_M | TIMERB_TIMEOUT_EVENT_M;
    TIMER1_IMR_R |= 0x200 | TIMERB_MATCH_EVENT_M | TIMERB_TIMEOUT_EVENT_M;

    IntRegister(INT_TIMER1B, TIMER1B_Handler);
    NVIC_EN0_R |= 0x400000;

    IntMasterEnable();
}

void servo_cal(void) {
    /*servo_move(90);
    servo_move(180);
    if(PREVIOUS_ANGLE==180){
        MAX_CLOCK_COUNT =  CLOCK_CYCLE_COUNT;
    }
    servo_move(0);
    if(PREVIOUS_ANGLE == 0){
        MIN_CLOCK_COUNT = CLOCK_CYCLE_COUNT*.0000001;
    }*/

}

void servo_move(int16_t degrees){
    // Given zero rad and pi rad, I know that, for example, 90 degrees is at zero_rad + (pi_rad - zero_rad) / 2
    // one_degree = (pi_rad - zero_rad) / 180
    // match_register = degrees * one_degree + zero_rad
    // wait a little bit
    if(degrees > 180){
        degrees = 180;
    }

    if(SERVO_DIRECTION == CLOCKWISE){
        if(PREVIOUS_ANGLE <= 0){
            return;
        }

    }else{
        if(PREVIOUS_ANGLE >= 180){
            return;
        }
    }
    /*if((degrees + SERVO_BIAS) < (0 + SERVO_BIAS) || (degrees + SERVO_BIAS) > (180 + SERVO_BIAS)){
        return;
    }*/
    //degrees += SERVO_BIAS;
    PREVIOUS_ANGLE = degrees;
    //uint32_t high_period_ns = ((0.0005/90) * degrees + 0.0008) * 1000000000;
    //uint32_t high_period_ns = (((30000-8000)/360) * degrees + 8000);
    uint32_t high_period_ns = (((.0019)/180) * degrees +.0005) * 1000000000; //closest
    //.0025-.0007

    //0.0000133

    CLOCK_CYCLE_COUNT = high_period_ns / CLOCK_CYCLE_LENGTH_NS;
    TIMER1_TBMATCHR_R = (PWM_PERIOD_LENGTH - CLOCK_CYCLE_COUNT) & 0xFFFF;
    TIMER1_TBPMR_R = ((PWM_PERIOD_LENGTH - CLOCK_CYCLE_COUNT) & 0xFF0000) >> 16;

    TIMER1_TBPR_R &= 0x0;
    TIMER1_TBILR_R &= 0x0;

    TIMER1_TBPR_R |= (PWM_PERIOD_LENGTH & 0xFF0000) >> 16;
    TIMER1_TBILR_R |= (PWM_PERIOD_LENGTH & 0xFFFF);

    TIMER1_CTL_R |= 0x100;

}

void set_servo_direction(SERVO_DIRECTION_E servo_direction){
    SERVO_DIRECTION = servo_direction;
}

SERVO_DIRECTION_E get_servo_direction(){
    return SERVO_DIRECTION;
}

void set_servo_bias(int16_t current_angle){
    SERVO_BIAS = current_angle - 90;
}

int16_t get_servo_bias(){
    return SERVO_BIAS;
}

int32_t get_CLOCK_CYCLE_COUNT(){
    return CLOCK_CYCLE_COUNT;
}

void TIMER1B_Handler(){
}
