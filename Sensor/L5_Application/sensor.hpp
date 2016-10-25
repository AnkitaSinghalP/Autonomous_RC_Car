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
void isr1();
void isr2();
void isr3();
int sensor_sonar_middle_trigger();
int sensor_sonar_right_trigger();
int sensor_sonar_left_trigger();
int sensor_sonar_rear_trigger();
int sensor_sonar_rear_left_trigger();


#endif /* L5_APPLICATION_SENSOR_HPP_ */
