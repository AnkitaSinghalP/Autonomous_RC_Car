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
//#include "_can_dbc//generated_can.h"
#include <stdio.h>
#include "can.h"
#include "string.h"
#include <uart2.hpp>
#include "tasks.hpp"
#include "examples/examples.hpp"

#include <math.h>
#include "i2c2.hpp"
#include "i2c_base.hpp"
#include "i2c2_device.hpp"
#include "utilities.h"
#include "lpc_pwm.hpp"
#include "gpio.hpp"
#include "eint.h"
#include "navigation.h"

GEO_DIRECTION_t geo_direction = {GEO_STOP};


GPIO *flag = NULL;
/// This is the stack size used for each of the period tasks (1Hz, 10Hz, 100Hz, and 1000Hz)
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

Navigation nav;
bool sys_cmd_flag = false;

/**
 * This is the stack size of the dispatcher task that triggers the period tasks to run.
 * Minimum 1500 bytes are needed in order to write a debug file if the period tasks overrun.
 * This stack size is also used while calling the period_init() and period_reg_tlm(), and if you use
 * printf inside these functions, you need about 1500 bytes minimum
 */
const uint32_t PERIOD_DISPATCHER_TASK_STACK_SIZE_BYTES = (512 * 3);

GEO_HEARTBEAT_t geo_heartbeat = { 0 };

bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
    can_msg_t can_msg = { 0 };
    can_msg.msg_id = mid;
    can_msg.frame_fields.data_len = dlc;
    memcpy(can_msg.data.bytes, bytes, dlc);

    return CAN_tx(can1, &can_msg, 0);
}

/*
 * test code starts here
 */

//BLE_CHCK_PT_t p1,p2,p3,p4,p5,p6,p7,last;
//
//vector<BLE_CHCK_PT_t> dummy_checkpoints;

/*****test code ends here*****/

/// Called once before the RTOS is started, this is a good place to initialize things once
bool period_init(void)
{
    nav.gps_init();
    nav.compass_init();

    CAN_init(can1, 100, 200, 200, 0, 0);
    CAN_bypass_filter_accept_all_msgs();
    CAN_reset_bus(can1);

/*
 * test code starts here
 * this is for testing purpose only
 */
//    p1.BLE_CHCK_PT_lat  = 37.335497;//37.336039;//37.335510;
//    p1.BLE_CHCK_PT_long = -121.883331;//-121.881883;//-121.881462;
//
//    p2.BLE_CHCK_PT_lat  = 37.335945;//37.335735;//37.335361;
//    p2.BLE_CHCK_PT_long = -121.883651;//-121.881691;//-121.881363;
//
//    p3.BLE_CHCK_PT_lat  = 37.335568;//37.336039;//37.335510;
//    p3.BLE_CHCK_PT_long = -121.884418;//-121.881883;//-121.881462;
//
//
//    p4.BLE_CHCK_PT_lat  = 37.334307;//37.335758;
//    p4.BLE_CHCK_PT_long = -121.883516;//-121.881638;
//
//
//    p5.BLE_CHCK_PT_lat  = 37.335013;//37.335745;//37.336025;
//    p5.BLE_CHCK_PT_long = -121.882022;//-121.882736;//-121.881660;
//
//    p6.BLE_CHCK_PT_lat  = 37.335295;//37.337059;//37.336189;
//    p6.BLE_CHCK_PT_long = -121.881274;//-121.880859;//-121.881790;
//
//    p7.BLE_CHCK_PT_lat  = 37.336267;
//    p7.BLE_CHCK_PT_long = -121.882001;
//
//
//    last.BLE_CHCK_PT_lat = 0;
//    last.BLE_CHCK_PT_long = 0;
//
//    dummy_checkpoints.push_back(p1);
//    dummy_checkpoints.push_back(p2);
//    dummy_checkpoints.push_back(p3);
//    dummy_checkpoints.push_back(p4);
//    dummy_checkpoints.push_back(p5);
//    dummy_checkpoints.push_back(p6);
//    dummy_checkpoints.push_back(p7);
//
//    dummy_checkpoints.push_back(last);

/*
 * test code ends here
 */

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


can_msg_t can_msg_received;
dbc_msg_hdr_t can_msg_hdr;
MASTER_SYSTEM_CMD_t systemcmd = {SYSTEM_STOP};
BLE_CHCK_PT_t ble_chck_pt = {1,1};

void period_1Hz(uint32_t count)
{

    if(CAN_is_bus_off(can1))
    	CAN_reset_bus(can1);

    geo_heartbeat.GEO_HEARTBEAT_tx_bytes = nav.all_checkpoints.size();
	dbc_encode_and_send_GEO_HEARTBEAT(&geo_heartbeat);

    //printf("GPS -: %f  %f\n",nav.coordinates.latitude,nav.coordinates.longitude);
    //printf("Distance = %f(feet)\n\n", nav.gps_distance);
	//printf("Compass= %d\n\n",nav.compass_angle);
	//printf("Bearing = %d\n",nav.gps_bearing_angle);


	//decode all received messages here
	while(CAN_rx(can1, &can_msg_received, 0))
	{
		can_msg_hdr.dlc = can_msg_received.frame_fields.data_len;
		can_msg_hdr.mid = can_msg_received.msg_id;

		dbc_decode_MASTER_SYSTEM_CMD(&systemcmd,can_msg_received.data.bytes,&can_msg_hdr);
		dbc_decode_BLE_CHCK_PT(&ble_chck_pt,can_msg_received.data.bytes,&can_msg_hdr);
		/*if(dbc_decode_BLE_CHCK_PT(&ble_chck_pt,can_msg_received.data.bytes,&can_msg_hdr))
		{
			if((int)ble_chck_pt.BLE_CHCK_PT_lat)
				printf("lat = %f   long = %f\n",ble_chck_pt.BLE_CHCK_PT_lat,ble_chck_pt.BLE_CHCK_PT_long);

		}*/
	}

	if(((int)ble_chck_pt.BLE_CHCK_PT_lat != 0) && ((int)ble_chck_pt.BLE_CHCK_PT_long != 0))
	{
		if(((int)ble_chck_pt.BLE_CHCK_PT_lat != 1) && ((int)ble_chck_pt.BLE_CHCK_PT_long != 1))
		{
			ble_chck_pt.BLE_CHCK_PT_long = 0 - ble_chck_pt.BLE_CHCK_PT_long;
			nav.next_checkpoint = {ble_chck_pt.BLE_CHCK_PT_lat,ble_chck_pt.BLE_CHCK_PT_long};

			nav.last_checkpoint_received = false;

			if(nav.all_checkpoints.size() != 0)
			{
				if((nav.next_checkpoint.latitude != nav.all_checkpoints[nav.all_checkpoints.size()-1].latitude)
						&&(nav.next_checkpoint.longitude != nav.all_checkpoints[nav.all_checkpoints.size()-1].longitude))
					//push to vectors
					nav.all_checkpoints.push_back(nav.next_checkpoint);
				else
					nav.last_checkpoint_received = true;
			}
			else
				nav.all_checkpoints.push_back(nav.next_checkpoint);

		}
	}
	else
	{
			if(nav.all_checkpoints.size())
			{
			    LE.on(4);
			    nav.last_checkpoint_received = true;
			}


	}


/*	if(nav.last_checkpoint_received)
		printf("number of checkpoints received= %d\n",nav.all_checkpoints.size());*/


}

void period_10Hz(uint32_t count)
{

	switch(systemcmd.MASTER_SYSTEM_CMD_enum)
	{
		case SYSTEM_STOP:
			sys_cmd_flag = false;
			LE.on(2);


/* commenting this section just for the testing
 *
 			nav.destination_reached = false;

			nav.last_checkpoint_received = false;
 			if(!nav.all_checkpoints.empty())
				nav.all_checkpoints.clear();
			*/

			break;
		case SYSTEM_START:
			sys_cmd_flag = true;
			LE.on(3);
			break;
		default :
			sys_cmd_flag = false;
			break;

	}

/*test code starts here*/
//	sys_cmd_flag = true;
//
//	if(!dummy_checkpoints.empty())
//	{
//		ble_chck_pt = dummy_checkpoints[0];
//		dummy_checkpoints.erase(dummy_checkpoints.begin());
//	}


/*test code ends here*/


	if(!nav.geo())
		nav.gps_fix = false;

    if(sys_cmd_flag && nav.gps_fix)
    {

		GEO_LOCATION_t geo_location;
		geo_location.GEO_LOCATION_lat  = nav.coordinates.latitude;
		geo_location.GEO_LOCATION_long = nav.coordinates.longitude;

		GEO_COMPASS_t geo_compass;
		geo_compass.GEO_COMPASS_mag = nav.compass_angle;


    	dbc_encode_and_send_GEO_LOCATION(&geo_location);
    	dbc_encode_and_send_GEO_COMPASS(&geo_compass);

    	if(nav.last_checkpoint_received)
    	{

    		if(geo_direction.GEO_DIRECTION_enum == DIR_STRAIGHT)
    			LD.setNumber(DIR_STRAIGHT);
    		if(geo_direction.GEO_DIRECTION_enum == DIR_HALF_LEFT)
    			LD.setNumber(DIR_HALF_LEFT);
    		if(geo_direction.GEO_DIRECTION_enum == DIR_LEFT)
    			LD.setNumber(DIR_LEFT);
    		if(geo_direction.GEO_DIRECTION_enum == DIR_HALF_RIGHT)
    			LD.setNumber(DIR_HALF_RIGHT);
    		if(geo_direction.GEO_DIRECTION_enum == DIR_RIGHT)
    			LD.setNumber(DIR_RIGHT);

/*    		if(nav.steer == 0)
    			printf("Go straight\n");
    		if(nav.steer == 1)
    			printf("Half left\n");
    		if(nav.steer == 2)
    			printf("Full left\n");
    		if(nav.steer == 3)
    			printf("Half right\n");
    		if(nav.steer == 4)
    			printf("Full right\n");*/

			//geo_direction.GEO_DIRECTION_enum = nav.steer;
    		dbc_encode_and_send_GEO_DIRECTION(&geo_direction);

			GEO_DEST_RCHD_t geo_dest_rchd;
			geo_dest_rchd.GEO_DEST_RCHD_stat = nav.destination_reached;

			dbc_encode_and_send_GEO_DEST_RCHD(&geo_dest_rchd);
    	}
    }
	else
	{
		if(!nav.gps_fix)
		{
			LD.setNumber(6);
		}
		else
		{
			geo_direction.GEO_DIRECTION_enum = GEO_STOP;
			dbc_encode_and_send_GEO_DIRECTION(&geo_direction);
			LD.setNumber(GEO_STOP);
		}

	}

}

void period_100Hz(uint32_t count)
{
   // LE.toggle(3);
}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{
    //LE.toggle(4);
}
