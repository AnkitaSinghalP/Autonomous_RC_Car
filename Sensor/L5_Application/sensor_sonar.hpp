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
        uint8_t middle_distance = 0 ;
        uint8_t rear_distance = 0 ;
        uint8_t right_distance  = 0;
        uint8_t left_distance = 0 ;
}sensor_readings;

void sensor_init();
void sensor_measure();
void can_communication_sensor();
//void received_sensor_can_msg();
void decoded_can_sensor_message(uint8_t middle,uint8_t left, uint8_t right, uint8_t rear);
void can_init_sensor();
void ultrasonic_sensor_heartbeat_message();
uint8_t received_sensor_can_msg();



#endif /* L5_APPLICATION_SENSOR_SONAR_HPP_ */
