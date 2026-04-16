/*
 * lab3.h
 *
 *  Created on: Feb 12, 2024
 *      Author: annieo10
 */

#ifndef LAB_3_H
#define LAB_3_H

#include "cyBot_Scan.h"
#include "math.h"
#include "movement.h"
#include "uart-interrupt.h"


#define SCAN_ARR_LEN_0_90_ 46
#define SCAN_ARR_LEN_0_45_ 26
#define SCAN_ARR_LEN_0_180_ 91
#define SCAN_ARR_LEN_90_180_ 46
#define SCAN_ARR_LEN_45_90_ 26
#define SCAN_ARR_LEN_45_180_ 69

#define SCAN_SENSOR_COUNT 2 // used in cyBot_Scan_Range as number of cols in matrix input
#define PING 0 // index of ping sensor in cyBot_Scan_Range matrix
#define IR 1 // index of ir sensor in cyBot_Scan_Range matrix

#define SCAN_ARR_LEN_A_B_(A, B) ((B-A) + 1)

size_t scan_arr_len_a_b_(int, int);

typedef uint8_t object_id_t;

typedef struct{
    object_id_t _object_id;

    uint8_t angle;

    float distance;

    uint8_t radial_width;
    float width;
} object_information_t;

typedef struct{
    uint8_t _objects_len;
    object_information_t* _objects[10];
} objects_t;


float average_float(float*,size_t);

uint8_t average_int(uint8_t*, size_t);

uint8_t within_distance(float, float);

float scan_data_object_cutoff(float[][2],size_t,int,int);

void cyBot_objects_inrange(objects_t*,int,int);

void cyBot_point_smallest_width(objects_t*, object_information_t*);

void cyBot_Scan_Range_PING(float*,int,int,int);

void cyBot_Scan_Range_IR(float*,int,int,int);

void cyBot_Scan_Range(float[][2],int,int,int);

void free_objects(objects_t*);

void init_object_info_struct(object_information_t*);

void init_objects_struct(objects_t*);

void copy_object_information(object_information_t*,object_information_t*);


#endif






