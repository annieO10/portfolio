#ifndef ADC_H
#define ADC_H

#include <inttypes.h>
#include <inc/tm4c123gh6pm.h>
#include <math.h>

void adc_init();
uint16_t adc_read();
float adc_to_distance(uint16_t);

#endif
