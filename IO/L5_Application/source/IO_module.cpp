/*
 * IO_module.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: BHUSHAN
 */

#include "IO_module.hpp"
#include "stdio.h"
#include "_can_dbc/generated_can.h"
#include "uart3.hpp"
#include "can.h"
#include "string.h"

SYSTEM_CMD_t system_cmd;
MOTOR_CMD_t motor_cmd = {0};
SYSTEM_STATUS_t system_status = {0};
SENSOR_BATT_t battery_status = {0};
BLE_MAP_START_DATA_t ble_map_start_data = {0};
BLE_MAP_DEST_DATA_t ble_map_destination_data = {0};
GEO_DEST_RCHD_t GEO_DEST_RCHD = {0};
MOTOR_SPEED_t motor_speed = {0};
IO_HEARTBEAT_t io_heart_beat_msg;
can_msg_t can_msg = { 0 };
dbc_msg_hdr_t can_msg_hdr;
GEO_LOCATION_t geo_location = {0};

const uint32_t             SYSTEM_CMD__MIA_MS = 1000;
const SYSTEM_CMD_t         SYSTEM_CMD__MIA_MSG = {SYSTEM_STOP};
const uint32_t             MOTOR_CMD__MIA_MS = 1000;
const MOTOR_CMD_t          MOTOR_CMD__MIA_MSG = {};
const uint32_t             SYSTEM_STATUS__MIA_MS = 1000;
const SYSTEM_STATUS_t      SYSTEM_STATUS__MIA_MSG = {};
const uint32_t             SENSOR_BATT__MIA_MS = 1000;
const SENSOR_BATT_t        SENSOR_BATT__MIA_MSG = {};
const uint32_t             BLE_MAP_START_DATA__MIA_MS = 1000;
const BLE_MAP_START_DATA_t BLE_MAP_START_DATA__MIA_MSG = {};
const uint32_t             BLE_MAP_DEST_DATA__MIA_MS = 1000;
const BLE_MAP_DEST_DATA_t  BLE_MAP_DEST_DATA__MIA_MSG = {};
const uint32_t             GEO_DEST_RCHD__MIA_MS = 1000;
const GEO_DEST_RCHD_t      GEO_DEST_RCHD__MIA_MSG = {};
const uint32_t             GEO_LOCATION__MIA_MS = 1000;
const GEO_LOCATION_t       GEO_LOCATION__MIA_MSG = {};
const uint32_t             MOTOR_SPEED__MIA_MS = 1000;
const MOTOR_SPEED_t        MOTOR_SPEED__MIA_MSG = {};

Uart3 &u3 = Uart3::getInstance();

char *received;

bool period_init(void)
{
	u3.init(38400,100,100);
    return true; // Must return true upon success
}


bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{

	can_msg.msg_id = mid;
	can_msg.frame_fields.data_len = dlc;
	memcpy(can_msg.data.bytes, bytes, dlc);
	return CAN_tx(can1, &can_msg, 0);
}

void can_init_IO()
{
	CAN_init(can1, 100, 128, 256,0,0);
	CAN_reset_bus(can1);
	CAN_bypass_filter_accept_all_msgs();

}


void SEND_IO_HEARTBEAT()
{

	dbc_encode_and_send_IO_HEARTBEAT(&io_heart_beat_msg);
}


void RECEIVED_SYSTEM_CMD()
{
	while(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_SYSTEM_CMD(&system_cmd, can_msg.data.bytes, &can_msg_hdr);

	}


	if(dbc_handle_mia_SYSTEM_CMD(&system_cmd, 10))
	{

	}

}

void RECEIVED_MOTOR_CMD()
{
	while(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_MOTOR_CMD(&motor_cmd, can_msg.data.bytes, &can_msg_hdr);

	}


	if(dbc_handle_mia_MOTOR_CMD(&motor_cmd, 10))
	{

	}

}

void RECEIVED_SYSTEM_STATUS()
{
	while(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_SYSTEM_STATUS(&system_status, can_msg.data.bytes, &can_msg_hdr);

	}


	if(dbc_handle_mia_SYSTEM_STATUS(&system_status, 10))
	{

	}

}


void RECEIVED_SENSOR_BATTERY()
{
	while(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_SENSOR_BATT(&battery_status, can_msg.data.bytes, &can_msg_hdr);

	}


	if(dbc_handle_mia_SENSOR_BATT(&battery_status, 10))
	{

	}

}

void RECEIVED_BLE_MAP_START_DATA()
{
	while(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_BLE_MAP_START_DATA(&ble_map_start_data, can_msg.data.bytes, &can_msg_hdr);

	}


	if(dbc_handle_mia_BLE_MAP_START_DATA(&ble_map_start_data, 10))
	{

	}

}

void RECEIVED_BLE_MAP_DESTINATION_DATA()
{
	while(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_BLE_MAP_DEST_DATA(&ble_map_destination_data, can_msg.data.bytes, &can_msg_hdr);

	}


	if(dbc_handle_mia_BLE_MAP_DEST_DATA(&ble_map_destination_data, 10))
	{

	}

}

void RECEIVED_GEO_DEST_RCHD()
{
	while(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_GEO_DEST_RCHD(&GEO_DEST_RCHD, can_msg.data.bytes, &can_msg_hdr);

	}


	if(dbc_handle_mia_GEO_DEST_RCHD(&GEO_DEST_RCHD, 10))
	{

	}

}

void RECEIVED_GEO_LOCATION()
{
	while(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_GEO_LOCATION(&geo_location, can_msg.data.bytes, &can_msg_hdr);

	}


	if(dbc_handle_mia_GEO_LOCATION(&geo_location, 10))
	{

	}

}

void RECEIVED_MOTOR_SPEED()
{
	while(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_MOTOR_SPEED(&motor_speed, can_msg.data.bytes, &can_msg_hdr);

	}


	if(dbc_handle_mia_MOTOR_SPEED(&motor_speed, 10))
	{

	}

}

void SEND_MSG_LCD()
{
	//u3.getChar(1);
	u3.putChar('H',1);
}
