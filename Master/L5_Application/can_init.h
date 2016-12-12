/*
 * can_init.h
 *
 *  Created on: Nov 12, 2016
 *      Author: ankita
 */

#ifndef L5_APPLICATION_CAN_INIT_H_
#define L5_APPLICATION_CAN_INIT_H_

#include "can.h"
#include "_can_dbc/generated_can.h"

extern BLE_HEARTBEAT_t ble_heartbeat_cmd;
extern SENSOR_HEARTBEAT_t sensor_heartbeat_cmd;
extern GEO_HEARTBEAT_t geo_heartbeat_cmd;
extern MOTOR_HEARTBEAT_t motor_heartbeat_cmd;
extern IO_HEARTBEAT_t io_heartbeat_cmd;

extern MASTER_MOTOR_CMD_t motor_cmd_message;
extern SENSOR_ULTRASONIC_t sensor_ultrasonic_cmd;
extern MASTER_SYSTEM_STATUS_t system_status_message;
extern BLE_COMM_CMD_t ble_comm_cmd;
extern MASTER_SYSTEM_CMD_t system_cmd_message;
extern GEO_DIRECTION_t geo_direction_cmd;
extern GEO_DEST_RCHD_t geo_dest_rchd_cmd;

extern can_msg_t can_msg;
extern int mia_count;


#endif /* L5_APPLICATION_CAN_INIT_H_ */
