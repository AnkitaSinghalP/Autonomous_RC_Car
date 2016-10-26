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
#include "io.hpp"
#include "stdio.h"
#include "timers.h"
#include "utilities.h"
#include "file_logger.h"
#include "can.h"
#include "_can_dbc/generated_can.h"
#include "lpc_timers.h"


MASTER_HEARTBEAT_t master_heartbeat_message = {0};
can_msg_t can_msg_master = { 0 };

const uint32_t            BLE_CMD__MIA_MS = 1000;
const BLE_HEARTBEAT_t      BLE_CMD__MIA_MSG = { 0 };
const uint32_t            SENSOR_CMD__MIA_MS = 1000;
const SENSOR_HEARTBEAT_t      SENSOR_CMD__MIA_MSG = { 0 };
const uint32_t            GEO_CMD__MIA_MS = 1000;
const GEO_HEARTBEAT_t      GEO_CMD__MIA_MSG = { 0 };
const uint32_t            MOTOR_CMD__MIA_MS = 1000;
const MOTOR_HEARTBEAT_t      MOTOR_CMD__MIA_MSG = { 0 };
const uint32_t            IO_CMD__MIA_MS = 1000;
const IO_HEARTBEAT_t      IO_CMD__MIA_MSG = { 0 };

BLE_HEARTBEAT_t ble_heartbeat_cmd = { 0 };
SENSOR_HEARTBEAT_t sensor_heartbeat_cmd = { 0 };
GEO_HEARTBEAT_t geo_heartbeat_cmd = { 0 };
MOTOR_HEARTBEAT_t motor_heartbeat_cmd = { 0 };
IO_HEARTBEAT_t io_heartbeat_cmd = { 0 };


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
	CAN_init(can1, 200, 500, 500,0,0);
	CAN_reset_bus(can1);
	CAN_bypass_filter_accept_all_msgs();
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
    LE.toggle(1);
}

void period_10Hz(uint32_t count)
{

	master_heartbeat_message.MASTER_HEARTBEAT_tx_bytes = count;
	master_heartbeat_message.MASTER_HEARTBEAT_rx_bytes = count;
	dbc_encode_and_send_MASTER_HEARTBEAT(&master_heartbeat_message);

	can_msg_t can_msg;

		     while(CAN_rx(can1, &can_msg, 0))
		    {

		        dbc_msg_hdr_t can_msg_hdr;
		        can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		        can_msg_hdr.mid = can_msg.msg_id;

		        if(dbc_decode_BLE_HEARTBEAT(&ble_heartbeat_cmd, can_msg.data.bytes, &can_msg_hdr))
		        {
		        	printf("%x  ", can_msg.data.bytes[0]);
		        }

		        if(dbc_decode_SENSOR_HEARTBEAT(&sensor_heartbeat_cmd, can_msg.data.bytes, &can_msg_hdr))
		        {
		        	printf("%x  ", can_msg.data.bytes[0]);
		        }

		        if(dbc_decode_GEO_HEARTBEAT(&geo_heartbeat_cmd, can_msg.data.bytes, &can_msg_hdr))
		        {
		        	printf("%x  ", can_msg.data.bytes[0]);
		        }
		        if(dbc_decode_MOTOR_HEARTBEAT(&motor_heartbeat_cmd, can_msg.data.bytes, &can_msg_hdr))
		        {
		             printf("%x  ", can_msg.data.bytes[0]);
		         }
		        if(dbc_decode_IO_HEARTBEAT(&io_heartbeat_cmd, can_msg.data.bytes, &can_msg_hdr))
		        {
		             printf("%x  ", can_msg.data.bytes[0]);
		        }


		    }



    LE.toggle(2);

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
