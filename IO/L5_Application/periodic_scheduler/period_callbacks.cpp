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

/// This is the stack size used for each of the period tasks (1Hz, 10Hz, 100Hz, and 1000Hz)
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

/**
 * This is the stack size of the dispatcher task that triggers the period tasks to run.
 * Minimum 1500 bytes are needed in order to write a debug file if the period tasks overrun.
 * This stack size is also used while calling the period_init() and period_reg_tlm(), and if you use
 * printf inside these functions, you need about 1500 bytes minimum
 */
const uint32_t PERIOD_DISPATCHER_TASK_STACK_SIZE_BYTES = (512 * 3);

const uint32_t                             SYSTEM_CMD__MIA_MS = 1000;
const SYSTEM_CMD_t                         SYSTEM_CMD__MIA_MSG= {SYSTEM_STOP};
const uint32_t                             MOTOR_CMD__MIA_MS= 1000;
const MOTOR_CMD_t                          MOTOR_CMD__MIA_MSG = {0};
const uint32_t                             SYSTEM_STATUS__MIA_MS= 1000;
const SYSTEM_STATUS_t                      SYSTEM_STATUS__MIA_MSG= {0,0,0,0,0,0,0,0};
const uint32_t                             SENSOR_BATT__MIA_MS = 1000;
const SENSOR_BATT_t                        SENSOR_BATT__MIA_MSG= {0};
const uint32_t                             BLE_MAP_DATA__MIA_MS = 1000;
const BLE_MAP_DATA_t                       BLE_MAP_DATA__MIA_MSG ={0,0,0,0};
const uint32_t                             GEO_DEST_RCHD__MIA_MS = 1000;
const GEO_DEST_RCHD_t                      GEO_DEST_RCHD__MIA_MSG={0};
const uint32_t                             GEO_LOCATION__MIA_MS = 1000;
const GEO_LOCATION_t                       GEO_LOCATION__MIA_MSG={0.0, 0.0,0};
const uint32_t                             MOTOR_SPEED__MIA_MS = 1000;
const MOTOR_SPEED_t                        MOTOR_SPEED__MIA_MSG= {0.0};

static int mia_count=0;
can_msg_t rx_msg = { 0 };

dbc_msg_hdr_t rx_msg_hdr;
 SYSTEM_CMD_t     system_cmd_rx;
 MOTOR_CMD_t      motor_cmd_rx;
 SYSTEM_STATUS_t  system_status_rx;
 SENSOR_BATT_t    sensor_batt_rx;
 BLE_MAP_DATA_t   ble_map_data_rx;
 GEO_DEST_RCHD_t  geo_dest_rchd_rx;
 GEO_LOCATION_t   geo_location_rx;
 MOTOR_SPEED_t    motor_speed_rx;
bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
    can_msg_t can_msg = { 0 };
    can_msg.msg_id                = mid;
    can_msg.frame_fields.data_len = dlc;
    memcpy(can_msg.data.bytes, bytes, dlc);

    return CAN_tx(can1, &can_msg, 0);
}
/// Called once before the RTOS is started, this is a good place to initialize things once
bool period_init(void)
{
    LD.setNumber(0);
    CAN_init(can1, 100,20,20,NULL,NULL);
    CAN_bypass_filter_accept_all_msgs();
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
    if(CAN_is_bus_off(can1)){
        CAN_reset_bus(can1);
    }
    /*******************************sending heartbeat********************************/
        IO_HEARTBEAT_t hbeat={0};
        hbeat.IO_HEARTBEAT_rx_bytes= 6;
        hbeat.IO_HEARTBEAT_tx_bytes= count;
        dbc_encode_and_send_IO_HEARTBEAT(&hbeat);
    /*******************************sending heartbeat********************************/
    //LE.toggle(1);
}

void period_10Hz(uint32_t count)
{


    /*******************************read messages********************************/
    while(CAN_rx(can1,&rx_msg, 0 )){

        rx_msg_hdr.dlc=rx_msg.frame_fields.data_len;
        rx_msg_hdr.mid=rx_msg.msg_id;
        dbc_decode_SYSTEM_CMD(&system_cmd_rx,rx_msg.data.bytes,&rx_msg_hdr);
     /*   dbc_decode_MOTOR_CMD(&motor_cmd_rx,rx_msg.data.bytes,&rx_msg_hdr);
        dbc_decode_SYSTEM_STATUS(&system_status_rx,rx_msg.data.bytes,&rx_msg_hdr);
        dbc_decode_SENSOR_BATT(&sensor_batt_rx,rx_msg.data.bytes,&rx_msg_hdr);
        dbc_decode_BLE_MAP_DATA(&ble_map_data_rx,rx_msg.data.bytes,&rx_msg_hdr);
        dbc_decode_GEO_DEST_RCHD(&geo_dest_rchd_rx,rx_msg.data.bytes,&rx_msg_hdr);
        LE.toggle(3);
        dbc_decode_GEO_LOCATION(&geo_location_rx,rx_msg.data.bytes,&rx_msg_hdr);
        dbc_decode_MOTOR_SPEED(&motor_speed_rx,rx_msg.data.bytes,&rx_msg_hdr);*/
    }

    /*******************************read messages********************************/
   if(dbc_handle_mia_SYSTEM_CMD(&system_cmd_rx,10) ){
       LD.setNumber(mia_count);
       mia_count++;
       //LE.toggle(2);
       if(mia_count==99){
           mia_count=0;
       }
   }
//    dbc_handle_mia_SENSOR_BATT(&sensor_batt_rx,10);
//    dbc_handle_mia_BLE_MAP_DATA(&ble_map_data_rx,10);
//    dbc_handle_mia_GEO_DEST_RCHD(&geo_dest_rchd_rx,10);
//    dbc_handle_mia_GEO_LOCATION(&geo_location_rx,10);
//    dbc_handle_mia_MOTOR_CMD(&motor_cmd_rx,10);
//    dbc_handle_mia_MOTOR_SPEED(&motor_speed_rx,10);
//    dbc_handle_mia_SYSTEM_STATUS(&system_status_rx,10);


    //LE.toggle(2);
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
