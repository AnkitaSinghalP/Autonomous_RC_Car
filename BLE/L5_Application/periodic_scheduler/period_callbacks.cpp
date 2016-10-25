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


bool flag1;
can_std_id_t id;
bool flag_tx;
can_msg_t abc;
<<<<<<< HEAD

MASTER_HEARTBEAT_t master_heartbeat = { 0 };
SYSTEM_STATUS_t system_status = { 0 };
SYSTEM_CMD_t system_cmd = {0,1,2};
SENSOR_SPEED_t sensor_speed = { 0 };
SENSOR_BATT_t sensor_batt = { 0 };
GEO_LOCATION_t geo_location = { 0 };
can_msg_t can_msg;

BLE_HEARTBEAT_t ble_heartbeat_t = { 0 };
BLE_COMM_CMD_t ble_cmd = { 0 };
BLE_CHCK_PT_t ble_chk_pt = { 0 };
BLE_DEST_RCHD_t ble_dest_rchd = { 0 };
BLE_MAP_DATA_t ble_map_data = { 0 };
=======
>>>>>>> added BLE_heartbeat

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
<<<<<<< HEAD

=======
>>>>>>> added BLE_heartbeat
	flag1 = CAN_init(can1, 100, 100, 100, 0, 0);
	if(flag1!=true){
    printf("CAN init failed \n");
	}
	CAN_reset_bus(can1);
<<<<<<< HEAD
	CAN_bypass_filter_accept_all_msgs();
=======
>>>>>>> added BLE_heartbeat
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
    can_msg_t can_msg = { 0 };
    can_msg.msg_id = mid;
    can_msg.frame_fields.data_len = dlc;
    memcpy(can_msg.data.bytes, bytes, dlc);
    return CAN_tx(can1, &can_msg, 0);
}

void period_1Hz(uint32_t count)
{
	if(CAN_is_bus_off(can1)){
	CAN_reset_bus(can1);
	}
<<<<<<< HEAD

	while(CAN_rx(can1, &can_msg, 0))
			    {
			      dbc_msg_hdr_t can_msg_hdr;
			      can_msg_hdr.dlc = can_msg.frame_fields.data_len;
			      can_msg_hdr.mid = can_msg.msg_id;



			      if(dbc_decode_SYSTEM_STATUS(&system_status, can_msg.data.bytes, &can_msg_hdr)){
			    	  for(int i =0; i<2; i++){
			    	  printf("%x  ", can_msg.data.bytes[i]);
			      }
			      }

			      if(dbc_decode_SENSOR_SPEED(&sensor_speed, can_msg.data.bytes, &can_msg_hdr)){
			    	  printf("%x  ", can_msg.data.bytes[0]);
			      }

			      if(dbc_decode_SENSOR_BATT(&sensor_batt, can_msg.data.bytes, &can_msg_hdr)){
			    	  printf("%x  ", can_msg.data.bytes[0]);
			      }


			      if(dbc_decode_SYSTEM_CMD(&system_cmd, can_msg.data.bytes, &can_msg_hdr)){
			    	  printf("%x  ", can_msg.data.bytes[0]);
			      }

			      LE.off(0);
			      LE.off(1);
			      LE.off(2);
			      LE.off(3);// switch off led if message is received
		     }

		 if(dbc_handle_mia_SYSTEM_CMD(&system_cmd, 10)){
			static int count = 1;
			LD.setNumber(count);
			count++;
			LE.toggle(0);
		 		      }

		 if(dbc_handle_mia_SYSTEM_STATUS(&system_status, 10)){
		 	static int count2 = 1;
		 	LD.setNumber(count2);
		 	count2++;
		 	LE.toggle(2);
		   }

		 if(dbc_handle_mia_SENSOR_SPEED(&sensor_speed, 10)){
		 	static int count3 = 1;
		 	LD.setNumber(count3);
		 	count3++;
		 	LE.toggle(3);
		   }

		 if(dbc_handle_mia_SENSOR_BATT(&sensor_batt, 10)){
		 	 	static int count4 = 1;
		 	 	LD.setNumber(count4);
		 	 	count4++;
		 	 	//LE.toggle(3);
		 	   }


=======
>>>>>>> added BLE_heartbeat
}

void period_10Hz(uint32_t count)
{
<<<<<<< HEAD


		ble_heartbeat_t.BLE_HEARTBEAT_tx_bytes = 0x8;
		ble_heartbeat_t.BLE_HEARTBEAT_rx_bytes = 0x7;
		ble_cmd.BLE_COMM_CMD_enum = 0x11;
		ble_chk_pt.BLE_CHCK_PT_lat = 0x1234;
		ble_chk_pt.BLE_CHCK_PT_long = 0x5678;
		ble_dest_rchd.BLE_DEST_RCHD_stat = 0x12;
		ble_map_data.BLE_MAP_DATA_dest_lat = 0x1111;
		ble_map_data.BLE_MAP_DATA_dest_long = 0x2222;
		ble_map_data.BLE_MAP_DATA_start_lat = 0x3333;
		ble_map_data.BLE_MAP_DATA_start_long = 0x4444;

=======
	    BLE_HEARTBEAT_t ble_heartbeat_t = { 0 };
		ble_heartbeat_t.BLE_HEARTBEAT_tx_bytes = 0x8;
>>>>>>> added BLE_heartbeat
		//ble_heartbeat_t.BLE_HEARTBEAT_rx_bytes = 0x5;

	    printf("can_msg");
	    if(dbc_encode_and_send_BLE_HEARTBEAT(&ble_heartbeat_t)){
<<<<<<< HEAD
	    printf("can_msg tx %d \n", ble_heartbeat_t.BLE_HEARTBEAT_tx_bytes);
	    printf("can_msg rx %d \n", ble_heartbeat_t.BLE_HEARTBEAT_rx_bytes);
	    }

	    if(dbc_encode_and_send_BLE_COMM_CMD(&ble_cmd)){
	   	    printf("can_msg %d \n", ble_cmd.BLE_COMM_CMD_enum);
	   	    }

	    if(dbc_encode_and_send_BLE_CHCK_PT(&ble_chk_pt)){
	   	    printf("can_msg %d \n", ble_chk_pt.BLE_CHCK_PT_lat);
	   	    printf("can_msg %d \n", ble_chk_pt.BLE_CHCK_PT_long);
	   	    }

	    if(dbc_encode_and_send_BLE_DEST_RCHD(&ble_dest_rchd)){
	   	    printf("can_msg %d \n", ble_dest_rchd.BLE_DEST_RCHD_stat);
	   	    }

	    if(dbc_encode_and_send_BLE_MAP_DATA(&ble_map_data)){
	   	    printf("can_msg %d \n", ble_map_data.BLE_MAP_DATA_dest_lat);
	   	    printf("can_msg %d \n", ble_map_data.BLE_MAP_DATA_dest_long);
	   	    printf("can_msg %d \n", ble_map_data.BLE_MAP_DATA_start_lat);
	   	    printf("can_msg %d \n", ble_map_data.BLE_MAP_DATA_start_long);
	   	    }
=======
	    printf("can_msg %d \n", ble_heartbeat_t.BLE_HEARTBEAT_tx_bytes);
	    }

>>>>>>> added BLE_heartbeat
}

void period_100Hz(uint32_t count)
{

	while(CAN_rx(can1, &can_msg, 0)){

		 dbc_msg_hdr_t can_msg_hdr;
		 can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		 can_msg_hdr.mid = can_msg.msg_id;

		 if(dbc_decode_GEO_LOCATION(&geo_location, can_msg.data.bytes, &can_msg_hdr)){
			for(int i =0; i<5; i++){
		    printf("%x  ", can_msg.data.bytes[i]);
		}
	}

		 if( dbc_decode_MASTER_HEARTBEAT(&master_heartbeat, can_msg.data.bytes, &can_msg_hdr)){
		    for(int i =0; i<4; i++){
		    printf("%x  ", can_msg.data.bytes[i]);
		 }
	}
	}

	 if(dbc_handle_mia_GEO_LOCATION(&geo_location, 10)){
	    static int count5 = 1;
		LD.setNumber(count5);
		count5++;
	   }

	 if(dbc_handle_mia_MASTER_HEARTBEAT(&master_heartbeat, 10)){
	   static int count1 = 1;
	   LD.setNumber(count1);
	   count1++;
	   LE.toggle(1);
	 }

    //LE.toggle(3);
}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{
    LE.toggle(4);
}
