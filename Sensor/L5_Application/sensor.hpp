/*

 * sensor.hpp

 *

 *  Created on: Oct 18, 2016

 *      Author: BHUSHAN

 */



#ifndef L5_APPLICATION_SENSOR_HPP_

#define L5_APPLICATION_SENSOR_HPP_



#include<stdio.h>

#include <stdint.h>

#include "lpc_timers.h"

#include "eint.h"

#include "io.hpp"

#include "gpio.hpp"

#include"timers.h"

#include "utilities.h"



void sensor_init();

void isr_middle1();

void isr_right1();

void isr_left1();

void isr_rear1();

void isr_middle();

void isr_right();

void isr_left();

void isr_rear();



int sensor_sonar_middle_trigger();

int sensor_sonar_right_trigger();

int sensor_sonar_left_trigger();

int sensor_sonar_rear_trigger();

//int sensor_sonar_rear_left_trigger();





#endif /* L5_APPLICATION_SENSOR_HPP_ */

