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

#include <heartbeat.hpp>
#include <heartbeat.hpp>
#include <stdint.h>
#include <system_cmd.hpp>
#include "io.hpp"
#include "periodic_callback.h"
#include "stdio.h"
#include "timers.h"
#include "utilities.h"
#include "file_logger.h"
#include "lpc_timers.h"
#include "string.h"
#include "can_init.h"
#include "free_run.hpp"
#include "system_cmd.hpp"

//Heart beat CAN Messages
const uint32_t BLE_HEARTBEAT__MIA_MS = 500;
const BLE_HEARTBEAT_t BLE_HEARTBEAT__MIA_MSG = { 0 };
const uint32_t            SENSOR_HEARTBEAT__MIA_MS = 200;
const SENSOR_HEARTBEAT_t      SENSOR_HEARTBEAT__MIA_MSG = { 0 };
const uint32_t            GEO_HEARTBEAT__MIA_MS = 200;
const GEO_HEARTBEAT_t      GEO_HEARTBEAT__MIA_MSG = { 0 };
const uint32_t            MOTOR_HEARTBEAT__MIA_MS = 200;
const MOTOR_HEARTBEAT_t      MOTOR_HEARTBEAT__MIA_MSG = { 0 };
const uint32_t            IO_HEARTBEAT__MIA_MS = 500;
const IO_HEARTBEAT_t      IO_HEARTBEAT__MIA_MSG = { 0 };

const uint32_t            BLE_COMM_CMD__MIA_MS = 1000;
const BLE_COMM_CMD_t      BLE_COMM_CMD__MIA_MSG = {COMM_STOP};
const uint32_t            SENSOR_ULTRASONIC__MIA_MS = 1000;
const SENSOR_ULTRASONIC_t   SENSOR_ULTRASONIC__MIA_MSG = { 0 };
const uint32_t            SENSOR_BATT__MIA_MS = 1000;
const SENSOR_BATT_t       SENSOR_BATT__MIA_MSG = { 0 };
const uint32_t                 GEO_DIRECTION__MIA_MS = 1000;
const GEO_DIRECTION_t          GEO_DIRECTION__MIA_MSG = { 0 };
const uint32_t                 GEO_DEST_RCHD__MIA_MS = 1000;
const GEO_DEST_RCHD_t          GEO_DEST_RCHD__MIA_MSG = { 0 };

MASTER_SYSTEM_CMD_t system_cmd_message = {SYSTEM_STOP};
MASTER_SYSTEM_STATUS_t system_status_message = {0};
BLE_COMM_CMD_t ble_comm_cmd = {COMM_STOP};
SENSOR_BATT_t sensor_batt_status = { 0 };
GEO_DIRECTION_t geo_direction_cmd = { 0 };
GEO_DEST_RCHD_t geo_dest_rchd_cmd = { 0 };
SENSOR_ULTRASONIC_t sensor_ultrasonic_cmd = { 0 };
MASTER_MOTOR_CMD_t motor_cmd_message = {0};
BLE_HEARTBEAT_t ble_heartbeat_cmd = { 0 };
SENSOR_HEARTBEAT_t sensor_heartbeat_cmd = { 0 };
GEO_HEARTBEAT_t geo_heartbeat_cmd = { 0 };
MOTOR_HEARTBEAT_t motor_heartbeat_cmd = { 0 };
IO_HEARTBEAT_t io_heartbeat_cmd = { 0 };


can_msg_t can_msg;
int mia_count;

bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
    can_msg.msg_id                = mid;
    can_msg.frame_fields.data_len = dlc;
    memcpy(can_msg.data.bytes, bytes, dlc);

    return CAN_tx(can1, &can_msg, 0);
}

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
bool period_init(void)
{
	//Initializing the CAN bus
	CAN_init(can1, 100, 200, 200,0,0);
	CAN_bypass_filter_accept_all_msgs();
	CAN_reset_bus(can1);

	//Initializing the Motor Command to move forward
	motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
	motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_FORWARD;
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
	//Reset bus in case of bus off
	 if(CAN_is_bus_off(can1))
	 	{
	 		CAN_reset_bus(can1);
	 	}

	//Receive Heart beat messages and calculate % bus utilization
	 heartbeat_rx();
}

void period_10Hz(uint32_t count)
{
	//Recieve system command from BLE and send system commands accordingly
	sys_cmd();

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
