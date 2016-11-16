/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

/**
 * @file
 * This contains the period callback functions for the periodic scheduler
 *
 * @warning
 * These callbacks should be used for hard real-time system, and the priority of these
 * tasks are above everything else in the system (above the PRIORITY_CRITICAL).
 * The period functions SHOULD NEVER block and SHOULD NEVER run over their time slot.
 * For example, the 1000Hz take slot runs periodically every 1ms, and whatever you
 * do must be completed within 1ms.  Running over the time slot will reset the system.
 */

#include <stdint.h>
#include "io.hpp"
#include "periodic_callback.h"
#include "_can_dbc//generated_can.h"
#include <stdio.h>
#include "can.h"
#include "string.h"
#include "gps.hpp"
#include <uart2.hpp>
#include "tasks.hpp"
#include "examples/examples.hpp"
#include "Compass.hpp"
#include "Compass_info.hpp"
#include <math.h>
#include "i2c2.hpp"
#include "i2c_base.hpp"
#include "i2c2_device.hpp"
#include "utilities.h"
#include "lpc_pwm.hpp"

#include "navigation.h"

/// This is the stack size used for each of the period tasks (1Hz, 10Hz, 100Hz, and 1000Hz)
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);


Navigation nav;

/*I2C2& i2c = I2C2::getInstance();

float resolution = 0.91;
int Mag_Angle;

char direction_command()
    {
    uint8_t xMHiByte = 0, xMLoByte = 0, yMHiByte =0, yMLoByte=0,zMHiByte=0, zMLoByte=0;
    int16_t xMagData =0, yMagData =0, zMagData=0;
    float xScaled, yScaled, resolution, headingRad;
    float heading = 0.0;

    i2c.writeReg(DEVICE_WRITE_ADDRESS,0x02,0x00);

    xMHiByte = i2c.readReg(DEVICE_READ_ADDRESS, 0x03);
    xMLoByte = i2c.readReg(DEVICE_READ_ADDRESS, 0x04);
    yMHiByte = i2c.readReg(DEVICE_READ_ADDRESS, 0x07);
    yMLoByte = i2c.readReg(DEVICE_READ_ADDRESS, 0x08);
    zMHiByte = i2c.readReg(DEVICE_READ_ADDRESS, 0x05);
    zMLoByte = i2c.readReg(DEVICE_READ_ADDRESS, 0x06);
    xMagData = (xMHiByte << 8) | xMLoByte;
    yMagData = (yMHiByte << 8) | yMLoByte;
    zMagData = (zMHiByte << 8) | zMLoByte;

    xScaled = xMagData * 0.91;
    yScaled = yMagData * 0.91;

    headingRad = (atan2(yScaled,xScaled));
    headingRad = headingRad + 0.2361; // radinas 13.63 declination

    heading = ((headingRad * 180)/Pi);
    if(heading < 0)
    {
       heading = 360 + heading;
    }
    Mag_Angle = heading;
    printf("Magnetomer_Angle:%d\n", Mag_Angle);
    return Mag_Angle;
    }*/
/**
 * This is the stack size of the dispatcher task that triggers the period tasks to run.
 * Minimum 1500 bytes are needed in order to write a debug file if the period tasks overrun.
 * This stack size is also used while calling the period_init() and period_reg_tlm(), and if you use
 * printf inside these functions, you need about 1500 bytes minimum
 */
const uint32_t PERIOD_DISPATCHER_TASK_STACK_SIZE_BYTES = (512 * 3);

bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
	can_msg_t can_msg = { 0 };
	can_msg.msg_id = mid;
	can_msg.frame_fields.data_len = dlc;
	memcpy(can_msg.data.bytes, bytes, dlc);

	return CAN_tx(can1, &can_msg, 0);
}

/// Called once before the RTOS is started, this is a good place to initialize things once
bool period_init(void)
{
	nav.gps_init();




   // CompassInit();
    //u2.init(9600,255,255);
	//u2.init(115200,255,255);

    //char *str= (char*)SET_OUTPUT_ALLDATA;
    //u2.put(SET_OUTPUT_RMC_GGA,0);
    //u2.put(UPDATE_RATE_10HZ,0);


    CAN_init(can1, 100, 20, 20, 0, 0);
    CAN_reset_bus(can1);
    return true; // Must return true upon success
}

/// Register any telemetry variables
bool period_reg_tlm(void)
{
    // Make sure "SYS_CFG_ENABLE_TLM" is enabled at sys_config.h to use Telemetry
    return true; // Must return true upon success
}


/**
 * Below are your periodic functions.
 * The argument 'count' is the number of times each periodic task is called.
 */

void period_1Hz(uint32_t count)
{

	CAN_is_bus_off(can1);
	CAN_reset_bus(can1);
	LE.toggle(1);
	uint8_t slaveAddr = SLAVE_ADDRESS;
	LPC_I2C2->I2ADR0 = slaveAddr;
	//direction_command();
	GEO_HEARTBEAT_t geo_heartbeat = { 0 };
	geo_heartbeat.GEO_HEARTBEAT_tx_bytes = 9;

	dbc_encode_and_send_GEO_HEARTBEAT(&geo_heartbeat);

/*	static int size = 255;

	static char *nmea = new char[size];

	char ch = '0';
	int index = 0;

	while(ch != '$')
	{
		u2.getChar(&ch,0);
		nmea[index] = ch;
		index ++;
	}

	if(gps(nmea))
		printf("\n");

	printf("raw: %s\n",nmea);*/

}

void period_10Hz(uint32_t count)
{

/*		static int size = 255;

		static char *nmea = new char[size];

		if(u2.gets(nmea,size,0))
		{
			if(gps(nmea))
				printf("\n");
			//printf("raw: %s\n",nmea);
		}*/

	if(nav.geo())
		printf("\n");

}

void period_100Hz(uint32_t count)
{
    LE.toggle(3);
}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{
    LE.toggle(4);
}
