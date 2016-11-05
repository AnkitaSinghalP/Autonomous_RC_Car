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
#include <stdio.h>
#include "_can_dbc/generated_can.h"
#include "string.h"
#include "LED_Display.hpp"


bool flag1;
can_std_id_t id;
//static bool flag_tx = true;
//can_msg_t abc;


SYSTEM_CMD_t system_cmd = {SYSTEM_STOP};
GEO_LOCATION_t geo_location = { 0 };
can_msg_t can_msg = { 0 };

BLE_HEARTBEAT_t ble_heartbeat_t = { 0 };
BLE_COMM_CMD_t ble_cmd = { 0 };
BLE_CHCK_PT_t ble_chk_pt = { 0 };
BLE_MAP_DATA_t ble_map_data = { 0 };
static int count1 = 0;

const uint32_t SYSTEM_CMD__MIA_MS = 1000;
const SYSTEM_CMD_t SYSTEM_CMD__MIA_MSG = { SYSTEM_STOP };

/*const uint32_t GEO_LOCATION__MIA_MS = 1000;
const GEO_LOCATION_t GEO_LOCATION__MIA_MSG = { 0 };*/
bool flag_tx;
can_msg_t abc;

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

	flag1 = CAN_init(can1, 100, 20, 20, 0, 0);
	CAN_reset_bus(can1);
	CAN_bypass_filter_accept_all_msgs();
	LD.setNumber(0);
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
	can_msg.msg_id = mid;
	can_msg.frame_fields.data_len = dlc;
	memcpy(can_msg.data.bytes, bytes, dlc);
	return CAN_tx(can1, &can_msg, 0);
}

void period_1Hz(uint32_t count)
{

	ble_heartbeat_t.BLE_HEARTBEAT_tx_bytes = 0x8;
	ble_heartbeat_t.BLE_HEARTBEAT_rx_bytes = 0x7;

	if(CAN_is_bus_off(can1)){
		CAN_reset_bus(can1);
	}

	dbc_encode_and_send_BLE_HEARTBEAT(&ble_heartbeat_t);

}

void period_10Hz(uint32_t count)
{
    ble_cmd.BLE_COMM_CMD_enum = 0x11;
	ble_chk_pt.BLE_CHCK_PT_lat = 0x1234;
	ble_chk_pt.BLE_CHCK_PT_long = 0x5678;
	ble_map_data.BLE_MAP_DATA_dest_lat = 0x1111;
	ble_map_data.BLE_MAP_DATA_dest_long = 0x2222;
	ble_map_data.BLE_MAP_DATA_start_lat = 0x3333;
	ble_map_data.BLE_MAP_DATA_start_long = 0x4444;

    dbc_encode_and_send_BLE_COMM_CMD(&ble_cmd);

	dbc_encode_and_send_BLE_CHCK_PT(&ble_chk_pt);

	dbc_encode_and_send_BLE_MAP_DATA(&ble_map_data);

	while(CAN_rx(can1, &can_msg, 0)){

			dbc_msg_hdr_t can_msg_hdr;
			can_msg_hdr.dlc = can_msg.frame_fields.data_len;
			can_msg_hdr.mid = can_msg.msg_id;

			dbc_decode_GEO_LOCATION(&geo_location, can_msg.data.bytes, &can_msg_hdr);
			dbc_decode_SYSTEM_CMD(&system_cmd, can_msg.data.bytes, &can_msg_hdr);
		}

//		if(dbc_handle_mia_GEO_LOCATION(&geo_location, 10)){
//			static int count5 = 0;
//			LD.setNumber(count5);
//			count5++;
			//LE.toggle(1);
//		}

		if(dbc_handle_mia_SYSTEM_CMD(&system_cmd, 10)){

			    LD.setNumber(count1);
				count1++;

				if(count1 == 99)
					count1 = 0;
				//LE.toggle(0);
			}


}



void period_100Hz(uint32_t count)
{




	//LE.toggle(3);
}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{
	LE.toggle(4);
}
