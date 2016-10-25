/*
 * Ir_sensor.cpp
 *
 *  Created on: Oct 25, 2016
 *      Author: BHUSHAN
 */

#include "stdio.h"
#include "Infrared_sensor.hpp"
#include "gpio.hpp"

GPIO *IR_sensor_pin = NULL;

void IR_sensor_init()
{
	if(IR_sensor_pin== NULL)
	{
		IR_sensor_pin = new GPIO(P2_0);
	}
	IR_sensor_pin->setAsInput();
}

int IR_sensor_trigger(){
	int speed= 0;
	static int count = 0;

	if(IR_sensor_pin->read() == 1)
	{
		++count;
	}

	return speed;
}
