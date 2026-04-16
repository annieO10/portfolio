/*
 * adc.c
 *
 *  Created on: Mar 26, 2024
 *      Author: nperk03
 */


#include "adc.h"

void adc_init(){
    uint32_t dummy_var = 0;

    SYSCTL_RCGCGPIO_R |= 0x2;
    dummy_var = SYSCTL_RCGCGPIO_R;
    SYSCTL_RCGCADC_R |= 0x1;
    dummy_var = SYSCTL_RCGCADC_R;
    GPIO_PORTB_DIR_R |= 0x10;
    GPIO_PORTB_DEN_R &= (~0x10);
    GPIO_PORTB_AMSEL_R |= 0x10;
    GPIO_PORTB_AFSEL_R |= 0x10;


    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN3;
    ADC0_CC_R = 0x0;
    ADC0_SSCTL3_R = 0x6;
    ADC0_SAC_R |= 0x4;
    ADC0_EMUX_R &= ~0xF000;
    ADC0_SSMUX3_R |= 0xA;
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN3;
}

uint16_t adc_read(){
    ADC0_PSSI_R = ADC_PSSI_SS3;
    while((ADC0_RIS_R & 0x8) == 0);
    uint16_t p = 0;
    p = ADC0_SSFIFO3_R;

    ADC0_ISC_R = ADC_ISC_IN3;

    uint32_t i = 0;
    for(i = 0; i < 50000; ++i){}

    return p;
}

float adc_to_distance(uint16_t data){
    return 198637 * (pow(data,-1.274));
}
