/*
 * button.h
 *
 *  Created on: Jul 18, 2016
 *      Author: Eric Middleton
 *
 * @edit: Phillip Jones 05/30/2019 : Removed uneeded helper functions
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>

#ifndef SYSCTL_RCGC2_GPIOE
#define SYSCTL_RCGC2_GPIOE 0x10
#endif

#ifndef direction
#define direction 0
#endif

#ifndef digital
#define digital 0xF
#endif


//initialize the push buttons
void button_init();


///Non-blocking call
///Returns highest value button being pressed, 0 if no button pressed
uint8_t button_getButton();


#endif /* BUTTON_H_ */
