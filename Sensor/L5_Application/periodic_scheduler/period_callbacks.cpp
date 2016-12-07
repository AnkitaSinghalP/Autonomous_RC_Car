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

//#include <sensor_sonar.hpp>
#include <stdint.h>
#include "io.hpp"
#include "periodic_callback.h"
#include "io.hpp"
#include "stdio.h"
#include "timers.h"
#include "utilities.h"
#include "file_logger.h"
#include "can.h"
#include "_can_dbc/generated_can.h"
#include "lpc_timers.h"
#include "string.h"
#include "eint.h"
#include "tasks.hpp"
SENSOR_HEARTBEAT_t sensor_heartbeat_message = {0};
//can_msg_t can_msg_sensor = { 0 };
can_msg_t can_msg_received;
can_msg_t can_msg = { 0 };
MASTER_SYSTEM_CMD_t master_command;

const uint32_t                             MASTER_SYSTEM_CMD__MIA_MS = 1000;
const MASTER_SYSTEM_CMD_t                  MASTER_SYSTEM_CMD__MIA_MSG = {SYSTEM_STOP};
SENSOR_ULTRASONIC_t ultrasonic_sensor_data = {0};

SENSOR_ULTRASONIC_t ultrasonic_sensor_receiver = {0};

dbc_mia_info_t mia_handling = {0};

SENSOR_BATT_t battery_status = {0};

dbc_msg_hdr_t can_msg_hdr;

static int counter = 0;
//SYSTEM_CMD_t master_cmd = {0};

//const uint32_t            MASTER_CMD__MIA_MS = 3000;
//const SYSTEM_CMD_t      MASTER_CMD__MIA_MSG = { 25 };


/// This is the stack size used for each of the period tasks (1Hz, 10Hz, 100Hz, and 1000Hz)
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

/**
 * This is the stack size of the dispatcher task that triggers the period tasks to run.
 * Minimum 1500 bytes are needed in order to write a debug file if the period tasks overrun.
 * This stack size is also used while calling the period_init() and period_reg_tlm(), and if you use
 * printf inside these functions, you need about 1500 bytes minimum
 */
const uint32_t PERIOD_DISPATCHER_TASK_STACK_SIZE_BYTES = (512 * 3);

/// Called once before the RTOS is started, this is a good place to initialize things once

bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{

	can_msg.msg_id = mid;
	can_msg.frame_fields.data_len = dlc;
	memcpy(can_msg.data.bytes, bytes, dlc);
	return CAN_tx(can1, &can_msg, 0);
}

bool period_init(void)
{
	//sensor_init();
	CAN_init(can1, 100, 20, 20,0,0);
	CAN_reset_bus(can1);
	CAN_bypass_filter_accept_all_msgs();
	LD.setNumber(0);

	// scheduler_add_task(new I2C_hapTask(PRIORITY_MEDIUM));
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

	if(CAN_is_bus_off(can1))
	{
		CAN_reset_bus(can1);
	}

	sensor_heartbeat_message.SENSOR_HEARTBEAT_tx_bytes = 12;
	sensor_heartbeat_message.SENSOR_HEARTBEAT_rx_bytes = 54;

	//printf("%d",count);
	//sensor_measure();
	dbc_encode_and_send_SENSOR_HEARTBEAT(&sensor_heartbeat_message);
	LE.toggle(1);
}

void period_10Hz(uint32_t count)
{

	/*ultrasonic_sensor_data.SENSOR_ULTRASONIC_middle = 0;
	ultrasonic_sensor_data.SENSOR_ULTRASONIC_left = ;
	ultrasonic_sensor_data.SENSOR_ULTRASONIC_right = ;
	ultrasonic_sensor_data.SENSOR_ULTRASONIC_rear = ;
	ultrasonic_sensor_data.SENSOR_ULTRASONIC_critical = ;*/

	dbc_encode_and_send_SENSOR_ULTRASONIC(&ultrasonic_sensor_data);

	battery_status.SENSOR_BATT_stat = 75;
	dbc_encode_and_send_SENSOR_BATT(&battery_status);
	//puts("sent");


	LE.toggle(2);

	while(CAN_rx(can1, &can_msg_received, 0))
	{
		can_msg_hdr.dlc = can_msg_received.frame_fields.data_len;
		can_msg_hdr.mid = can_msg_received.msg_id;

		dbc_decode_MASTER_SYSTEM_CMD(&master_command, can_msg_received.data.bytes, &can_msg_hdr);

	}

	if(dbc_handle_mia_MASTER_SYSTEM_CMD(&master_command, 10))
	{
		LD.setNumber(counter);
		counter++;
		LE.toggle(3);
		if(counter == 99 )
			counter =0;

	}

}

void period_100Hz(uint32_t count)
{
	//LE.toggle(3);


	// Empty all of the queued, and received messages within the last 10ms (100Hz callback frequency)

}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{
	//LE.toggle(4);
}



