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

extern MASTER_MOTOR_CMD_t motor_cmd_message;
extern SENSOR_ULTRASONIC_t sensor_ultrasonic_cmd;
extern can_msg_t can_msg;
extern int mia_count;

#endif /* L5_APPLICATION_CAN_INIT_H_ */
