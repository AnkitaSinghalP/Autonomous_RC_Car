/*
 * sensor.hpp
 *
 *  Created on: Oct 18, 2016
 *      Author: BHUSHAN
 */

#ifndef L5_APPLICATION_SENSOR_SONAR_HPP_
#define L5_APPLICATION_SENSOR_SONAR_HPP_
#include <stdint.h>


#define SENSOR_DELAY 20

typedef struct
{
        uint8_t middle_distance ;
        uint8_t rear_distance  ;
        uint8_t right_distance ;
        uint8_t left_distance  ;
}sensor_readings;

bool sensor_init();
bool sensor_measure();



#endif /* L5_APPLICATION_SENSOR_SONAR_HPP_ */
