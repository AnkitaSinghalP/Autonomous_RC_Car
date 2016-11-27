/*
 * IO_module.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: BHUSHAN
 */

#include "IO_module.hpp"
#include "stdio.h"
#include "_can_dbc/generated_can.h"
//#include "uart3.hpp"
#include "can.h"
#include "string.h"
#include "io.hpp"
#include "stdlib.h"
#include "utilities.h"
#include "uart3.hpp"
#include "gpio.hpp"
#include "string.h"


MASTER_SYSTEM_CMD_t system_cmd;
MASTER_MOTOR_CMD_t motor_cmd = {0};
MASTER_SYSTEM_STATUS_t system_status = {0};
SENSOR_BATT_t battery_status = {0};
BLE_MAP_START_DATA_t ble_map_start_data = {0};
BLE_MAP_DEST_DATA_t ble_map_destination_data = {0};
GEO_DEST_RCHD_t GEO_DEST_RCHD = {0};
MOTOR_SPEED_t motor_speed = {0};
IO_HEARTBEAT_t io_heart_beat_msg;
can_msg_t can_msg = { 0 };
dbc_msg_hdr_t can_msg_hdr;
GEO_LOCATION_t geo_location = {0};
SENSOR_ULTRASONIC_t ultrasonic_sensor = {0};

MASTER_SYSTEM_CMD_enum_E master_system_command;
MASTER_MOTOR_CMD_steer_E master_motor_command_steer;
MASTER_MOTOR_CMD_drive_E master_motor_command_drive;

const uint32_t             MASTER_SYSTEM_CMD__MIA_MS = 1000;
const MASTER_SYSTEM_CMD_t         MASTER_SYSTEM_CMD__MIA_MSG = {SYSTEM_STOP};
const uint32_t             MOTOR_CMD__MIA_MS = 1000;
const MASTER_MOTOR_CMD_t          MOTOR_CMD__MIA_MSG = {};
const uint32_t             SYSTEM_STATUS__MIA_MS = 1000;
const MASTER_SYSTEM_STATUS_t      SYSTEM_STATUS__MIA_MSG = {};
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

bool geo_flag = false;
bool geo_page_flag = false;
bool sensor_flag = false;
bool sensors_page_flag = false;
bool motor_flag = false;
bool motor_page_flag = false;
bool home_flag = false;
bool home_page_flag = false;
char lcd_cha;

float dc_slow = 6.35;
float dc_normal = 6.28;
float dc_turbo = 6.20;

LCD_SCREENS lcdscreen = home_message;
//Uart3 &u3 = Uart3::getInstance();
Uart3 &U3 = Uart3::getInstance();


static GPIO left_pin(P1_20);
static GPIO right_pin(P1_19);
static GPIO reverse_pin(P2_0);
static GPIO start_pin(P0_29);
static GPIO stop_pin(P0_29);
static GPIO brake_pin(P0_30);
static GPIO resume_pin(P0_30);
static GPIO headlight_pin(P1_22);
static GPIO system_start_pin(P1_23);
static GPIO system_stop_pin(P1_28);
static GPIO system_reset_pin(P1_29);

static char received_ack=0;                         // received_acknowledge byte from LCD

void flag_change(bool* flagp){
	geo_flag = false;
	home_flag = false;
	sensor_flag = false;
	motor_flag = false;
	*flagp = true;
}

void flag_page_change(bool* flagp){
	geo_page_flag = false;
	home_page_flag = false;
	sensors_page_flag = false;
	motor_page_flag = false;
	*flagp = true;
}


bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{

	can_msg.msg_id = mid;
	can_msg.frame_fields.data_len = dlc;
	memcpy(can_msg.data.bytes, bytes, dlc);
	return CAN_tx(can1, &can_msg, 0);
}
void SEND_MSG_LCD(char a,char b,char c, char d,char e) {
	char comm[6]={a,b,c,d,e,0};
	for(int i=0;i<5;i++) {
		comm[5]^=comm[i];
	}

	for(int i=0;i<6;i++) {
		U3.putChar(comm[i],10);
	}
	U3.getChar(&received_ack,10);
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


bool RECEIVED_SYSTEM_CMD()
{
	if(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_MASTER_SYSTEM_CMD(&system_cmd, can_msg.data.bytes, &can_msg_hdr);

		master_system_command = system_cmd.MASTER_SYSTEM_CMD_enum;

		if(master_system_command == SYSTEM_START)

		{
			SEND_MSG_LCD(0x01,0x0E,0x00,0x00,0x01);
			system_start_pin.setLow();
		}
		else if(master_system_command == SYSTEM_STOP)
		{
			SEND_MSG_LCD(0x01,0x0E,0x00,0x00,0x00);
			system_stop_pin.setLow();
		}
		else if(master_system_command == SYSTEM_RESET)
		{
			system_reset_pin.setLow();
		}


	}


	if(dbc_handle_mia_MASTER_SYSTEM_CMD(&system_cmd, 10))
	{

	}
	return true;
}
void RECEIVED_SENSOR_ULTRASONIC()
{
	if(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_SENSOR_ULTRASONIC(&ultrasonic_sensor, can_msg.data.bytes, &can_msg_hdr);

		if(ultrasonic_sensor.SENSOR_ULTRASONIC_left == 0)
			SEND_MSG_LCD(0x01,0x0B,0x03,0x00,0x32);
		else if(ultrasonic_sensor.SENSOR_ULTRASONIC_left == 1)
			SEND_MSG_LCD(0x01,0x0B,0x03,0x00,0x5A);

		if(ultrasonic_sensor.SENSOR_ULTRASONIC_right == 0)
			SEND_MSG_LCD(0x01,0x0B,0x02,0x00,0x32);
		else if(ultrasonic_sensor.SENSOR_ULTRASONIC_right == 1)
			SEND_MSG_LCD(0x01,0x0E,0x02,0x00,0x5A);

		if(ultrasonic_sensor.SENSOR_ULTRASONIC_middle == 0)
			SEND_MSG_LCD(0x01,0x0B,0x01,0x00,0x32);
		else if(ultrasonic_sensor.SENSOR_ULTRASONIC_middle == 1)
			SEND_MSG_LCD(0x01,0x0E,0x01,0x00,0x5A);

		if(ultrasonic_sensor.SENSOR_ULTRASONIC_rear == 0)
			SEND_MSG_LCD(0x01,0x0B,0x00,0x00,0x32);
		else if(ultrasonic_sensor.SENSOR_ULTRASONIC_rear == 1)
			SEND_MSG_LCD(0x01,0x0E,0x00,0x00,0x5A);

	}
	if(dbc_handle_mia_SENSOR_ULTRASONIC(&ultrasonic_sensor, 10))
	{

	}
}


void RECEIVED_MOTOR_CMD()
{
	if(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_MASTER_MOTOR_CMD(&motor_cmd, can_msg.data.bytes, &can_msg_hdr);

		switch( motor_cmd.MASTER_MOTOR_CMD_steer){
		case 0:
			left_pin.setLow();
			SEND_MSG_LCD(0x01,0x0E,0x02,0x00,0x01);
			break;
		case 1:

			left_pin.setLow();
			SEND_MSG_LCD(0x01,0x0E,0x02,0x00,0x01);
			break;
		case 2:
			right_pin.setLow();
			SEND_MSG_LCD(0x01,0x0E,0x01,0x00,0x01);
			break;
		case 3:
			right_pin.setLow();
			SEND_MSG_LCD(0x01,0x0E,0x01,0x00,0x01);
			break;
		case 5:
			reverse_pin.setLow();
			SEND_MSG_LCD(0x01,0x0E,0x03,0x00,0x01);
			break;
		default:
			right_pin.setHigh();
			left_pin.setHigh();
			reverse_pin.setHigh();
			SEND_MSG_LCD(0x01,0x0E,0x02,0x00,0x00);
			SEND_MSG_LCD(0x01,0x0E,0x01,0x00,0x00);
			SEND_MSG_LCD(0x01,0x0E,0x03,0x00,0x00);
		}
	}


	if(dbc_handle_mia_MASTER_MOTOR_CMD(&motor_cmd, 10))
	{

	}

}

void RECEIVED_SYSTEM_STATUS()
{
	if(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_MASTER_SYSTEM_STATUS(&system_status, can_msg.data.bytes, &can_msg_hdr);

		//printf("%f",system_status.MASTER_SYSTEM_STATUS_util);
	}


	if(dbc_handle_mia_MASTER_SYSTEM_STATUS(&system_status, 10))
	{

	}

}


void RECEIVED_SENSOR_BATTERY()
{
	if(CAN_rx(can1, &can_msg, 0))
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
	if(CAN_rx(can1, &can_msg, 0))
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
	if(CAN_rx(can1, &can_msg, 0))
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
	if(CAN_rx(can1, &can_msg, 0))
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
	if(CAN_rx(can1, &can_msg, 0))
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
	if(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_MOTOR_SPEED(&motor_speed, can_msg.data.bytes, &can_msg_hdr);
		SEND_MSG_LCD(0x01,0x08,0x00,0x00,motor_speed.MOTOR_SPEED_actual);
	}


	if(dbc_handle_mia_MOTOR_SPEED(&motor_speed, 10))
	{

	}

}

void LCD_init(void) {
	U3.init(LCD_BAUD_RATE,LCD_RXQSIZE,LCD_TXQSIZE);
	dc_slow = 6.35;
	dc_normal = 6.28;
	dc_turbo = 6.20;
	SEND_MSG_LCD(0x01,0x0A,0x00,0x00,0x00);

}
void IO_init()
{
	left_pin.setAsOutput();
	right_pin.setAsOutput();
	reverse_pin.setAsOutput();
	start_pin.setAsOutput();
	stop_pin.setAsOutput();
	brake_pin.setAsOutput();
	resume_pin.setAsOutput();
	headlight_pin.setAsOutput();
	system_start_pin.setAsOutput();
	system_stop_pin.setAsOutput();
	system_reset_pin.setAsOutput();


}

void put_string(char array[], uint8_t object_no, uint8_t num){
	uint8_t checksum=0x02^object_no^num;
	U3.putChar(0x02,10);
	U3.putChar(object_no,10);
	U3.putChar(num,10);

	for(int i=0;i<num;i++){
		U3.putChar(array[i],10);
		checksum^=array[i];
	}

	U3.putChar(checksum,10);
	U3.getChar(&received_ack,10);
}




void start(){
	SEND_IO_HEARTBEAT();
	RECEIVED_MOTOR_SPEED();
	RECEIVED_GEO_LOCATION();
	RECEIVED_GEO_DEST_RCHD();
	RECEIVED_BLE_MAP_DESTINATION_DATA();
	RECEIVED_BLE_MAP_START_DATA();
	RECEIVED_SENSOR_BATTERY();
	RECEIVED_SYSTEM_STATUS();
	RECEIVED_MOTOR_CMD();
	RECEIVED_SENSOR_ULTRASONIC();
}
