/*
* movement.c
*
*  Created on: Jan 30, 2024
*      Author: annieo10
*/
#include "movement.h"

double move_forward(oi_t*sensor_data,double distance_mm){

    double distance = 0;

    oi_setWheels(50,50);

    char _str[300];

    while(distance<distance_mm){
        oi_update(sensor_data);
        distance+=sensor_data->distance;

        memset(_str,0,300);
        snprintf(_str, 300, "[1,1,%.2f,%.2f,%.2f,%.2f,%u,%u,%u,%u]|", sensor_data->distance,sensor_data->angle,distance_mm,distance_mm,sensor_data->bumpLeft,sensor_data->bumpRight,sensor_data->cliffRight, sensor_data->cliffLeft);
        uart_sendStr(_str);

        if(command_flag == 14){
            break;
        }

    #ifdef LCD_DEBUG
    moving_lcd_debug(sensor_data, distance, 1);
    #endif

    }
    oi_setWheels(0,0);

    return distance;


}

double move_backward(oi_t*sensor_data, double distance_mm){
    double distance =0;

    oi_setWheels(-1*50, -1*50);

    char _str[300];

    while(distance>-1*distance_mm){
        oi_update(sensor_data);
        distance+=sensor_data->distance;

        memset(_str,0,300);
        snprintf(_str, 300, "[1,1,%.2f,%.2f,%.2f,%.2f,%u,%u,%u,%u]|", sensor_data->distance,sensor_data->angle,distance_mm,distance_mm,sensor_data->bumpLeft,sensor_data->bumpRight,sensor_data->cliffRight, sensor_data->cliffLeft);
        uart_sendStr(_str);

        #ifdef LCD_DEBUG
        moveing_lcd_debug(sensor_data,distance,0);
        #endif
    }

    oi_setWheels(0,0);

    return distance;

}

double turn_right(oi_t*sensor_data, double degrees){

    double degrees_turn = 0;

    oi_setWheels(-1*45,45);

    char _str[300];

    while(degrees_turn>-1*degrees){
        oi_update(sensor_data);
        degrees_turn+=sensor_data->angle;

        memset(_str,0,300);
        snprintf(_str, 300, "[1,1,%.2f,%.2f,%.2f,%.2f,%u,%u,%u,%u]|", sensor_data->distance,sensor_data->angle,(float)45,-1*(float)45,sensor_data->bumpLeft,sensor_data->bumpRight,sensor_data->cliffRight, sensor_data->cliffLeft);
        uart_sendStr(_str);

        #ifdef LCD_DEBUG
        turning_lcd_debug(sensor_data, degrees_turn, 0);
        #endif


    }
    oi_setWheels(0,0);

    return degrees_turn;

}

double turn_left(oi_t*sensor_data,double degrees){

    double degrees_turn = 0;

    oi_setWheels(45,-1*45);

    char _str[300];

    while(degrees_turn<degrees){
        oi_update(sensor_data);
        degrees_turn+=sensor_data->angle;

        memset(_str,0,300);
        snprintf(_str, 300, "[1,1,%.2f,%.2f,%.2f,%.2f,%u,%u,%u,%u]|", sensor_data->distance,sensor_data->angle,-1*(float)45,(float)45,sensor_data->bumpLeft,sensor_data->bumpRight,sensor_data->cliffRight, sensor_data->cliffLeft);
        uart_sendStr(_str);

        #ifdef LCD_DEBUG
        turning_lcd_debug(sensor_data, degrees_turn,1);
        #endif


    }
    oi_setWheels(0,0);

    return degrees_turn;

}

double drive_to_object(oi_t* sensor_data, float _object){

    char _str[300];

    double distance = 0;
    uint8_t collision = 0;
    oi_setWheels(60,60);

    double distance_to_travel = _object;

    while(distance<distance_to_travel){
        oi_update(sensor_data);


        memset(_str,0,300);
        snprintf(_str, 300, "[1,1,%.2f,%.2f,%.2f,%.2f,%u,%u,%u,%u]|", sensor_data->distance,sensor_data->angle,(float)60,(float)60,sensor_data->bumpLeft,sensor_data->bumpRight,sensor_data->cliffRight, sensor_data->cliffLeft);
        uart_sendStr(_str);

        if(command_flag == 14){
            break;
        }
        if(collision = collision_detected(sensor_data))
        {
            if(collision == 3){
                //Left Bumper Collision
                move_backward(sensor_data, 150);

                turn_right(sensor_data, 80);
                move_forward(sensor_data, 400);
                turn_left(sensor_data, 80);
                float _hyp = sqrt( ( pow((_object  - distance) + 150, 2) + 160000 ) );
                turn_left(sensor_data, (95 - (acos(250/_hyp) * (180/M_PI))));

                memset(_str,0,300);
                sprintf(_str, "[13,1,%.2f,%.2f,%.2f]|", _hyp,distance,(90 - (acos(250/_hyp) * (180/M_PI))));
                uart_sendStr(_str);

                _object = _hyp - 130;
                drive_to_object(sensor_data, _hyp);
                break;
            }
            else if (collision == 1){
                //Right Bumper Collision
                move_backward(sensor_data, 150);

                turn_left(sensor_data, 80);
                move_forward(sensor_data, 400);
                turn_right(sensor_data, 80);
                float _hyp = sqrt( ( pow((_object - distance) + 150, 2) + 160000 ) );
                turn_right(sensor_data, (95 - (acos(250/_hyp) * (180/M_PI))));

                memset(_str,0,300);
                sprintf(_str, "[13,0,%.2f,%.2f,%.2f]|", _hyp,distance,(90 - (acos(250/_hyp) * (180/M_PI))));
                uart_sendStr(_str);

                _object = _hyp - 130;
                drive_to_object(sensor_data, _hyp);
                break;
            }

        }
        else{
            distance+=sensor_data->distance;
        }

    }
    oi_setWheels(0,0);
    return distance;
}

/*
Collision with Left Bumper: 00000011
Collision with Right Bumper: 00000001
No Collision: 00000000
*/
uint8_t collision_detected(oi_t* sensor_data){
    if(sensor_data->bumpLeft == 0b1){
        return 3;
    }
    else if(sensor_data->bumpRight == 0b1){
        return 1;
    }
    return 0;
}

double move_forward_cd(oi_t* sensor_data, double distance_mm){
    double distance = 0;
    uint8_t collision = 0;
    oi_setWheels(distance_mm,distance_mm);

    while(distance<distance_mm){
        oi_update(sensor_data);
        if(command_flag == 14){
            break;
        }
        if(collision = collision_detected(sensor_data))
        {
            //lcd_printf("%d",collision);
            #ifdef LCD_DEBUG
            collision_lcd_debug(sensor_data);
            #endif
            if(collision == 3){
                //Left Bumper Collision
                move_backward(sensor_data, 150);
                distance -= 150;
                turn_right(sensor_data, 90);
                move_forward(sensor_data, 250);
                turn_left(sensor_data, 90);
                distance += move_forward_cd(sensor_data, distance_mm - distance);




            }
            else if (collision == 1){
                //Right Bumper Collision
                move_backward(sensor_data, 150);
                distance -= 150;
                turn_left(sensor_data, 90);
                move_forward(sensor_data, 250);
                turn_right(sensor_data, 90);
                distance += move_forward_cd(sensor_data, distance_mm - distance);

            }

        }
        else{
            distance+=sensor_data->distance;
        }

    }
    oi_setWheels(0,0);
    return distance;
}

#ifdef LCD_DEBUG

void turning_lcd_debug(oi_t* sensor_data, double distance_traveled, uint8_t direction){
    lcd_clear();
    if(direction){
        lcd_printf(“DIRECTION: F”);
    }
    else{
        lcd_printf(“DIRECTION: B”);
    }
    lcd_gotoLine(1);
    lcd_printf(“TRAVELED: %.4lf”,distance_traveled);
    lcd_gotoLine(2);
    lcd_printf(“DISTANCE: %.4lf”,sensor_data->distance);
}

void moving_lcd_debug(oi_t* sensor_data, double degrees_traveled, uint8_t direction){
    lcd_clear();
    if(direction){
        lcd_printf(“DIRECTION: L”);
    }
    else{
        lcd_printf(“DIRECTION: R”);
    }
    lcd_gotoLine(1);
    lcd_printf(“TRAVELED: %.4lf”,degrees_traveled);
    lcd_gotoLine(2);
    lcd_printf(“ANGLE: %.4lf”,sensor_data->angle);
}

void collision_lcd_debug(oi_t* sensor_data){
    lcd_clear();
    lcd_printf(“COLLISION”);
    lcd_gotoLine(1);
    if(sensor_data->bumpLeft){
        lcd_printf(“LOCATION: LEFT”);
    }
    else if(sensor_data->bumpRight){
        lcd_printf(“LOCATION: RIGHT”);
    }
}

#endif










