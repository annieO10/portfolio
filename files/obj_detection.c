 /* lab3.c
 *
 *  Created on: Feb 12, 2024
 *      Author: annieo10
 */

#include <obj_detection.h>

size_t scan_arr_len_a_b_(int begin, int end){
    return (((end-begin)/2) + 1);
}

void cyBot_Scan_Range_PING(float*_arr,int length, int b, int e){
    if(length > (((e-b)/2) + 1)){
        return;
    }

    cyBOT_Scan_t _data;
    int i = 0;
    for(i = 0; i < length; i += 1){
        cyBOT_Scan(b+(i*2), &_data);
        _arr[i] = _data.sound_dist;
    }
}

void cyBot_Scan_Range_IR(float*_arr,int length, int b, int e){
    if(length > (((e-b)/2) + 1)){
        return;
    }

    cyBOT_Scan_t _data;
    int i = 0;
    for(i = 0; i < length; i += 1){
        cyBOT_Scan(b+(i*2), &_data);
        _arr[i] = _data.IR_raw_val;
    }
}

void cyBot_Scan_Range(float _mat[][2], int length, int b, int e){
    if(length > (((e-b)/2) + 1)){
        return;
    }

    cyBOT_Scan_t _data;
    int i = 0;
    for(i = 0; i < length; i += 1){
        cyBOT_Scan(b+(i*2), &_data);
        _mat[i][PING] = _data.sound_dist;
        _mat[i][IR] = _data.IR_raw_val;
    }
}

float average_float(float *_arr, size_t _len){
    float sum = 0;
    int i = 0;
    for(i = 0; i < _len; ++i){
        sum += _arr[i];
    }
    return sum/((int) _len);
}

uint8_t average_int(uint8_t *_arr, size_t _len){
    uint16_t sum = 0;
    int i = 0;
    for(i = 0; i < _len; ++i){
        sum += _arr[i];
    }
    return sum/((uint8_t) _len);
}


uint8_t within_distance(float set_distance, float next_distance){
    if(set_distance * OBJECT_DIFFERENCE_TOLERANCE > next_distance || set_distance * (1 + (1-OBJECT_DIFFERENCE_TOLERANCE)) < next_distance){
        return 0;
    }
    return 1;
}

void cyBot_objects_inrange(objects_t* _objects_arr, int _begin, int _end){

    //Gather scan data and determine distance cutoff for objects
        size_t scan_data_length = scan_arr_len_a_b_(_begin,_end);
        float scan_data[scan_data_length][SCAN_SENSOR_COUNT];
        cyBot_Scan_Range(scan_data, scan_data_length, _begin, _end);
        float object_distance_cutoff = scan_data_object_cutoff(scan_data, scan_data_length, _begin, _end);
        _objects_arr->_objects_len = 0;
        int i = 0;
        for(i = 0; i < scan_data_length; ++i){
            if(_objects_arr->_objects_len < 10 && scan_data[i][IR] >= object_distance_cutoff){
                ++(_objects_arr->_objects_len);
                object_information_t* obj= (object_information_t*) calloc(1,sizeof(object_information_t));
                init_object_info_struct(obj);
                obj->_object_id = i;

                float distance_range[80];
                memset(distance_range, 0, 80 * sizeof(float));
                size_t distance_range_len = 0;

                uint8_t angle_range[80];
                memset(angle_range, 0, 80 * sizeof(uint8_t));
                size_t angle_range_len = 0;

                float ir_range[80];
                memset(ir_range, 0, 80 * sizeof(float));
                size_t ir_range_len = 0;


                angle_range[0] = _begin + i*2;
                ++(angle_range_len);

                distance_range[0] = scan_data[i][PING];
                ++(distance_range_len);

                ir_range[0] = scan_data[i][IR];
                ++(ir_range_len);

                ++i;

                while( i < scan_data_length && within_distance(ir_range[ir_range_len - 1], scan_data[i][IR])){
                    ++(ir_range_len);
                    ir_range[ir_range_len - 1] = scan_data[i][IR];


                    ++(angle_range_len);
                    angle_range[angle_range_len - 1] = _begin + i*2;

                    ++(distance_range_len);
                    distance_range[distance_range_len - 1] = scan_data[i][PING];

                    ++i;
                }

                obj->distance = average_float(distance_range, distance_range_len);
                obj->angle = average_int(angle_range, angle_range_len);
                obj->radial_width = angle_range[angle_range_len - 1] - angle_range[0];
                if(obj->radial_width == 0){
                    obj->radial_width = 1;
                }
                obj->width = 2 * M_PI * obj->distance * ((float)obj->radial_width / 360);

                if(obj->radial_width>OBJECT_RADIAL_WIDTH_CUTOFF){
                    _objects_arr->_objects[_objects_arr->_objects_len - 1] = obj;
                }else{
                    --(_objects_arr->_objects_len);
                    free(obj);
                }

            }
        }
}


void copy_object_information(object_information_t* _dest, object_information_t* _src){
    _dest->_object_id = _src->_object_id;
    _dest->angle = _src->angle;
    _dest->distance = _src->distance;
    _dest->radial_width = _src->radial_width;
    _dest->width = _src->width;
}

float scan_data_object_cutoff(float _arr[][2], size_t _len, int _begin, int _end){
    float shortest_distance = _arr[0][IR];
    int i = 0;
    for(i = 1; i < _len; ++i){
        if(_arr[i][IR] < shortest_distance){
            shortest_distance = _arr[i][IR];
        }
    }

    return shortest_distance * (1 + OBJECT_CUTOFF_TOLERANCE);
}

void cyBot_point_smallest_width(objects_t* _objs, object_information_t* _small){

    if(_objs->_objects_len == 0){
        return;
    }

    object_information_t smallest_object = {0};
    init_object_info_struct(&smallest_object);
    copy_object_information(&smallest_object, _objs->_objects[0]);
    int i = 0;
    for(i = 1; i < _objs->_objects_len; ++i){
        if(_objs->_objects[i]->width < smallest_object.width){
            copy_object_information(&smallest_object, _objs->_objects[i]);
        }
    }

    copy_object_information(_small, &smallest_object);

    cyBOT_Scan_t data;
    cyBOT_Scan(_small->angle, &data);
}

void init_object_info_struct(object_information_t* _data){
    _data->_object_id = 0;
    _data->angle = 0;
    _data->distance = 0;
    _data->radial_width = 0;
    _data->width = 0;
}

void init_objects_struct(objects_t* _data){
    _data->_objects_len = 0;
}

void free_objects(objects_t* _obj){
    int i = 0;
    for(i = 0; i < _obj->_objects_len; ++i){
        free(_obj->_objects[i]);
    }

}






