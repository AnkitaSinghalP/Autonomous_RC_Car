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
#include "_can_dbc/generated_can.h"
#include "stdio.h"
#include "can.h"
#include "string.h"

can_msg_t can_msg = { 0 };
can_msg_t msg_received;
MOTOR_CMD_t motorcmd={0};
SYSTEM_CMD_t systemcmd;
MOTOR_HEARTBEAT_t motorheartbeat;
MOTOR_SPEED_t motorspeed_actual;



const uint32_t        SYSTEM_CMD__MIA_MS = 1000;
const SYSTEM_CMD_t      SYSTEM_CMD__MIA_MSG  = {SYSTEM_STOP};
const uint32_t          MOTOR_CMD__MIA_MS =3000 ;
const MOTOR_CMD_t         MOTOR_CMD__MIA_MSG = {0};



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
	CAN_init(can1,100,128,256,0,0);
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

bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
	can_msg_t can_msg = {0};
	can_msg.msg_id = mid;
	can_msg.frame_fields.data_len = dlc;
	memcpy(can_msg.data.bytes, bytes, dlc);
	return CAN_tx(can1, &can_msg, 0);
}


void period_1Hz(uint32_t count)
{

	motorheartbeat.MOTOR_HEARTBEAT_tx_bytes=count;
	motorheartbeat.MOTOR_HEARTBEAT_rx_bytes=count;
	motorspeed_actual.MOTOR_SPEED_actual = 35.0;
	dbc_encode_and_send_MOTOR_HEARTBEAT(&motorheartbeat);
	dbc_encode_and_send_MOTOR_SPEED(&motorspeed_actual);
	/*if(CAN_tx(can1, &can_msg, 0))
			{
				printf("sent 0 %d\n",can_msg.data.bytes[0]);
				printf("sent 1 %f\n",can_msg.data.bytes[1]);
				printf("sent 2 %f\n",can_msg.data.bytes[2]);

			}*/



}

void period_10Hz(uint32_t count)
{
	static int countmia=0;
	CAN_bypass_filter_accept_all_msgs();
	if (CAN_rx(can1, &msg_received, 0))
	{
		dbc_msg_hdr_t can_msg_hdr;
		can_msg_hdr.dlc = msg_received.frame_fields.data_len;
		can_msg_hdr.mid = msg_received.msg_id;
		dbc_decode_MOTOR_CMD(&motorcmd,msg_received.data.bytes, &can_msg_hdr);
		dbc_decode_SYSTEM_CMD(&systemcmd,msg_received.data.bytes,&can_msg_hdr);
	}
	if(dbc_handle_mia_SYSTEM_CMD(&systemcmd, 100))
		countmia++;
	if(dbc_handle_mia_MOTOR_CMD(&motorcmd,1000))
		countmia++;
	if(countmia >99)
		countmia=0;
	LD.setNumber(countmia);
}

void period_100Hz(uint32_t count)
{

}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{
	LE.toggle(4);
}
