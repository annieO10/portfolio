/*
 * main.c
 *
 *  Created on: Feb 12, 2024
 *      Author: annieo10
 */


#include "uart-interrupt.h"
#include "lcd.h"
#include "Timer.h"
#include "button.h"
#include "open_interface.h"
#include "adc.h"
#include <string.h>
#include "ping.h"
#include "servo.h"
#include "button.h"

#define UART_STR_LENGTH 2000

int main(){
    timer_init();
    lcd_init();
    uart_interrupt_init();
//    cyBOT_init_Scan(0b111);
    ping_init();
    servo_init();
    oi_t* init_data = oi_alloc();
    button_init();
//    oi_init(init_data);
//    oi_setWheels(0,0);
    uart_sendStr("[0,1]|");

    int16_t current_angle = 90;
    uint8_t current_button = 0;
    uint8_t previous_button = 0;
    servo_move(90);
    lcd_printf("DIRECTION = %d\nANGLE = %d\nBIAS = %d",get_servo_direction(), current_angle, get_servo_bias());
    servo_cal();

    while(1){

        do{
            current_button = button_getButton();
        }while(current_button == 0);

        if(current_button == 1){
            if(get_servo_direction() == CLOCKWISE){
                if(current_angle <= 0 || (current_angle - 1) < 0){
                    current_angle = 0;
                }else{
                    current_angle -= 1;
                }
            }else{
                if(current_angle >= 180 || (current_angle + 1) > 180){
                    current_angle = 180;
                }else{
                    current_angle += 1;
                }
            }
//            if(!(current_angle - get_servo_bias() < 0 || current_angle - get_servo_bias() > 180)){
//                servo_move(current_angle);
//                current_angle -= get_servo_bias();
//            }
            servo_move(current_angle);
        }else if(current_button == 2){
            if(get_servo_direction() == CLOCKWISE){
                if(current_angle <= 0 || (current_angle - 5) < 0){
                    current_angle = 0;
                }else{
                    current_angle -= 5;
                }
            }else{
                if(current_angle >= 180 || (current_angle + 5) > 180){
                    current_angle = 180;
                }else{
                    current_angle += 5;
                }
            }
//            if(!(current_angle - get_servo_bias() < 0 || current_angle - get_servo_bias() > 180)){
//                servo_move(current_angle);
//                current_angle -= get_servo_bias();
//            }
            servo_move(current_angle);
        }else if(current_button == 3){
            if(previous_button != 3){
                if(get_servo_direction() == CLOCKWISE){
                    set_servo_direction(COUNTER_CLOCKWISE);
                }else{
                    set_servo_direction(CLOCKWISE);
                }

                            }
        }else if(current_button == 4){
            if(previous_button != 4){
                set_servo_bias(current_angle);
                current_angle -= get_servo_bias();
            }
        }
        previous_button = current_button;
        lcd_printf("DIRECTION = %d\nANGLE = %d\nBIAS = %d\n clock: %d",get_servo_direction(), current_angle, get_servo_bias(),get_CLOCK_CYCLE_COUNT());
        //lcd_printf("clock cycle: %d", get_CLOCK_CYCLE_COUNT());
        timer_waitMicros(100000);


    }

//    while(1){
//
//        if(command_flag == 2){
//
//            float _mat[SCAN_ARR_LEN_0_180_][SCAN_SENSOR_COUNT];
//
//            int i = 0;
//            for(i = 0; i < SCAN_ARR_LEN_0_180_; ++i){
//                memset(_mat[i], 0, SCAN_SENSOR_COUNT);
//            }
//
//            cyBot_Scan_Range(_mat, SCAN_ARR_LEN_0_180_, 0, 180);
//
//            char _str[UART_STR_LENGTH];
//            memset(_str, 0, UART_STR_LENGTH);
//
//            strncat(_str,"[2,",UART_STR_LENGTH);
//            for(i = 0; i < SCAN_ARR_LEN_0_180_ - 1; ++i){
//                char _data[12];
//                memset(_data, 0, 12);
//                snprintf(_data,12,"%.2f,",_mat[i][IR]);
//                strncat(_str,_data,UART_STR_LENGTH);
//            }
//
//            char _data[12];
//            memset(_data, 0, 12);
//            snprintf(_data,12,"%.2f]|",_mat[SCAN_ARR_LEN_0_180_ - 1][IR]);
//            strncat(_str,_data,UART_STR_LENGTH);
//
//            uart_sendStr(_str);
//
//        }else if(command_flag == 3){
//            float _mat[SCAN_ARR_LEN_0_180_][SCAN_SENSOR_COUNT];
//
//            int i = 0;
//            for(i = 0; i < SCAN_ARR_LEN_0_180_; ++i){
//                memset(_mat[i], 0, SCAN_SENSOR_COUNT);
//            }
//
//            cyBot_Scan_Range(_mat, SCAN_ARR_LEN_0_180_, 0, 180);
//
//            char _str[UART_STR_LENGTH];
//            memset(_str, 0, UART_STR_LENGTH);
//
//            strncat(_str,"[3,",UART_STR_LENGTH);
//            for(i = 0; i < SCAN_ARR_LEN_0_180_ - 1; ++i){
//                char _data[12];
//                memset(_data, 0, 12);
//                snprintf(_data,12,"%.2f,",_mat[i][PING]);
//                strncat(_str,_data,UART_STR_LENGTH);
//            }
//
//            char _data[12];
//            memset(_data, 0, 12);
//            snprintf(_data,12,"%.2f]|",_mat[SCAN_ARR_LEN_0_180_ - 1][PING]);
//            strncat(_str,_data,UART_STR_LENGTH);
//
//            uart_sendStr(_str);
//
//        }
//        else if(command_flag == 4){
//            float _mat[SCAN_ARR_LEN_0_180_][SCAN_SENSOR_COUNT];
//
//            int i = 0;
//            for(i = 0; i < SCAN_ARR_LEN_0_180_; ++i){
//                memset(_mat[i], 0, SCAN_SENSOR_COUNT);
//            }
//
//            objects_t objs_list = {0};
//            cyBot_objects_inrange(&objs_list, 0, 180);
//
//            char _str[UART_STR_LENGTH];
//            memset(_str, 0, UART_STR_LENGTH);
//
//            strncat(_str,"[4,",UART_STR_LENGTH);
//
//            char _data[10];
//            memset(_data, 0, 10);
//            snprintf(_data,10,"%d,",objs_list._objects_len);
//            strncat(_str,_data,UART_STR_LENGTH);
//
//
//            for(i = 0; i < objs_list._objects_len - 1; ++i){
//                char _data[30];
//                memset(_data, 0, 30);
//                snprintf(_data,30,"%d,%d,%.2f,%d,%.2f," ,objs_list._objects[i]->_object_id ,objs_list._objects[i]->angle ,objs_list._objects[i]->distance , objs_list._objects[i]->radial_width ,objs_list._objects[i]->width  );
//                strncat(_str,_data,UART_STR_LENGTH);
//
//            }
//
//            if(objs_list._objects_len > 0){
//                char _data[30];
//                memset(_data, 0, 30);
//                snprintf(_data,30,"%d,%d,%.2f,%d,%.2f" ,objs_list._objects[objs_list._objects_len - 1]->_object_id ,objs_list._objects[objs_list._objects_len-1]->angle ,objs_list._objects[objs_list._objects_len-1]->distance , objs_list._objects[objs_list._objects_len-1]->radial_width ,objs_list._objects[objs_list._objects_len-1]->width);
//                strncat(_str,_data,UART_STR_LENGTH);
//
//                cyBot_point_smallest_width(&objs_list, &smallest_object);
//                smallest_object_available = 1;
//            }
//
//            strncat(_str,"]|",UART_STR_LENGTH);
//
//            uart_sendStr(_str);
//
//            free_objects(&objs_list);
//        }else if(command_flag == 5){
//            char _str[30];
//            memset(_str,0,30);
//            snprintf(_str, 30, "[5,%.2f,%.2f,%u]|", OBJECT_CUTOFF_TOLERANCE, OBJECT_DIFFERENCE_TOLERANCE, OBJECT_RADIAL_WIDTH_CUTOFF);
//            uart_sendStr(_str);
//        }else if(command_flag == 6){
//            while((UART1_FR_R & 0x10) != 0);
//            move_forward(init_data, ((uint8_t)command_byte)*(float)100);
//            uart_sendStr("[6,1]|");
//        }else if(command_flag == 7){
//            while((UART1_FR_R & 0x10) != 0);
//            move_backward(init_data, ((uint8_t)command_byte)*(float)100);
//            uart_sendStr("[7,1]|");
//        }else if(command_flag == 8){
//            while((UART1_FR_R & 0x10) != 0);
//            turn_right(init_data, ((uint8_t)command_byte)*(float)10);
//            uart_sendStr("[8,1]|");
//        }else if(command_flag == 9){
//            while((UART1_FR_R & 0x10) != 0);
//            turn_left(init_data, ((uint8_t)command_byte)*(float)10);
//            uart_sendStr("[9,1]|");
//        }else if(command_flag == 10){
//            while((UART1_FR_R & 0x10) != 0);
//            char _str[30];
//            memset(_str,0,30);
//            snprintf(_str, 30, "[10,%u]|", ((uint8_t)command_byte));
//            uart_sendStr(_str);
//            OBJECT_CUTOFF_TOLERANCE = ((uint8_t)command_byte)/((float)40);
//            command_flag = 5;
//            continue;
//        }else if(command_flag == 11){
//            while((UART1_FR_R & 0x10) != 0);
//            char _str[30];
//            memset(_str,0,30);
//            snprintf(_str, 30, "[11,%u]|", ((uint8_t)command_byte));
//            uart_sendStr(_str);
//            OBJECT_DIFFERENCE_TOLERANCE = ((uint8_t)command_byte)/((float)40);
//            command_flag = 5;
//            continue;
//        }else if(command_flag == 12){
//            while((UART1_FR_R & 0x10) != 0);
//            char _str[30];
//            memset(_str,0,30);
//            snprintf(_str, 30, "[12,%u]|", ((uint8_t)command_byte));
//            uart_sendStr(_str);
//            OBJECT_RADIAL_WIDTH_CUTOFF = ((uint8_t)command_byte);
//            command_flag = 5;
//            continue;
//        }else if(command_flag == 13){
//            if(smallest_object_available){
//                //movement code
//                if(smallest_object.angle<95){
//                    turn_right(init_data,(360*smallest_object.width)/(2*M_PI*smallest_object.distance));
//
//                }
//                else if(smallest_object.angle>105){
//                    turn_left(init_data, (360*smallest_object.width)/(2*M_PI*smallest_object.distance));
//                }
//
//                smallest_object.distance = (smallest_object.distance * 10) - 80;
//
//                drive_to_object(init_data, (smallest_object.distance));
//
//
//                smallest_object_available = 0;
//                char _str[30];
//                memset(_str,0,30);
//                snprintf(_str, 30, "[13,%u,%.2f]|", smallest_object.radial_width,(smallest_object.distance * 10) - 80);
//                uart_sendStr(_str);
//            }else{
//                uart_sendStr("[13,0]|");
//            }
//        }else if(command_flag == 14){
//            uart_sendStr("[14,0]|");
//        }else if(command_flag == 15){
//            cyBOT_Scan_t _data = {0,0};
//            cyBOT_Scan(94, &_data);
//            adc_init();
//            uint16_t adc_data = adc_read();
//            float distance = adc_to_distance(adc_data);
//
//            char _str[30];
//            memset(_str,0,30);
//            snprintf(_str, 30, "[15,%u,%.2f]|", adc_data, distance);
//            uart_sendStr(_str);
//        }else if(command_flag == 16){
//            cyBOT_Scan_t _data = {0,0};
//            cyBOT_Scan(94, &_data);
//            float distance = ping_getDistance();
//
//            char _str3[30];
//            memset(_str3,0,30);
//            snprintf(_str3, 30, "[16,1,%.8f,%d]|", distance, CLOCK_TIMEOUT_COUNT);
//            uart_sendStr(_str3);
//        }else if(command_flag == 17){
//
//            servo_move_test(120);
//
//            char _str3[30];
//            memset(_str3,0,30);
//            snprintf(_str3, 30, "[17,1,120]|");
//            uart_sendStr(_str3);
//        }
//        command_flag = 0;
//    }

    oi_free(init_data);

}
