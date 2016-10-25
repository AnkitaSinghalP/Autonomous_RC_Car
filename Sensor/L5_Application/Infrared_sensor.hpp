/*
 * Ir_sensor.hpp
 *
 *  Created on: Oct 25, 2016
 *      Author: BHUSHAN
 */

#ifndef L5_APPLICATION_INFRARED_SENSOR_HPP_
#define L5_APPLICATION_INFRARED_SENSOR_HPP_

#include<stdio.h>
#include <stdint.h>
#include "lpc_timers.h"
#include "eint.h"
#include "io.hpp"
#include "gpio.hpp"
#include"timers.h"
#include "utilities.h"

void IR_sensor_init();
int IR_sensor_trigger();



#endif /* L5_APPLICATION_INFRARED_SENSOR_HPP_ */
