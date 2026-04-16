/*
 * movement.h
 *
 *  Created on: Jan 30, 2024
 *      Author: annieo10
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#include "obj_detection.h"
#include "open_interface.h"
#include "uart-interrupt.h"


double move_forward(oi_t*,double);

//move forward function that includes collision detection
double move_forward_cd(oi_t*,double);

double drive_to_object(oi_t*,float);

//helper function used to detect if the roomba has collided with an object
uint8_t collision_detected(oi_t*);

double move_backward(oi_t*,double);

double turn_right(oi_t*,double);

double turn_left(oi_t*,double);


//Debug functions
#ifdef LCD_BEBUG

void turning_lcd_debug(oi_t*,double,uint8_t);

void moving_lcd_debug(oi_t*, double, uint8_t);

void collision_lcd_debug(oi_t*);

#endif



#endif /* MOVEMENT_H_ */
