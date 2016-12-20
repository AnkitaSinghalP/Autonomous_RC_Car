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
#include "can.h"
#include<stdio.h>
#include "_can_dbc/generated_can.h"
#include <string.h>
//#include "uart3.hpp"
#include "utilities.h"
#include "gpio.hpp"
#include "IO_module.hpp"


bool screen1 = false;
bool screen2 = false;
bool screen3 = false;


/*extern bool destination_reached_flag;
extern bool system_command_flag;*/

IO_HEARTBEAT_t io_heart_beat_msg = {1};
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
can_msg_t can_msg;
bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{

	can_msg.msg_id = mid;
	can_msg.frame_fields.data_len = dlc;
	memcpy(can_msg.data.bytes, bytes, dlc);
	return CAN_tx(can1, &can_msg, 0);
}

bool period_init(void)
{
	can_init_IO();
	LCD_init();
	IO_init();
	SEND_MSG_LCD(0x01,SCREEN_VAL,0x00,0x00,0x00);
	//SEND_MSG_LCD(0x01,0x0A,0x03,0x00,0x00);

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

	static int screen_count;
	if(CAN_is_bus_off(can1)){
		CAN_reset_bus(can1);
	}
	io_heart_beat_msg.IO_HEARTBEAT_rx_bytes = count;
	io_heart_beat_msg.IO_HEARTBEAT_tx_bytes = count;
	dbc_encode_and_send_IO_HEARTBEAT(&io_heart_beat_msg);

	screen_count++;

	if(system_command_flag)
	{
		if(!(destination_reached_flag))
		{
			switch(screen_count)
			{
			case 5:
			case 10:
				SEND_MSG_LCD(0x01,0x0A,0x01,0x00,0x00);
				screen1 = true;
				screen3 = false;
				screen2 = false;
				LD.setNumber(1);
				break;
			case 15:
				SEND_MSG_LCD(0x01,0x0A,0x02,0x00,0x00);
				screen2 = true;
				screen1 = false;
				screen3 = false;
				LD.setNumber(2);
				/*break;
			case 20:
				SEND_MSG_LCD(0x01,0x0A,0x03,0x00,0x00);
				screen3 = true;
				screen1 = false;
				screen2 = false;
				LD.setNumber(3);*/
				screen_count =0;
				break;
			}
		}
		else
		{
			screen1 = false;
			screen3 = false;
			screen2 = false;
			SEND_MSG_LCD(ON,0x0A,0x04,OFF,OFF);
		}

	}


		if(screen1)
		{
			start_5sec();

		}
		else if(screen2)
		{
			start_10sec();

		}
		/*else if(screen3)
		{
			start_15sec();

		}*/






}

void period_10Hz(uint32_t count)
{

	start();

}

void period_100Hz(uint32_t count)
{

	//LE.toggle(3);
}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{


	//LE.toggle(4);
}
