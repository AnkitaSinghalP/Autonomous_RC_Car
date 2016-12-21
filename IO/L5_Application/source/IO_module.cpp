/*
 * IO_module.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: BHUSHAN
 */

#include "IO_module.hpp"
#include "stdio.h"
#include "_can_dbc/generated_can.h"
#include "can.h"
#include "string.h"
#include "io.hpp"
#include "stdlib.h"
#include "utilities.h"
#include "uart2.hpp"
#include "gpio.hpp"
#include "string.h"


MASTER_SYSTEM_CMD_t system_cmd;
MASTER_MOTOR_CMD_t motor_cmd = {};
MASTER_SYSTEM_STATUS_t system_status = {0};
SENSOR_BATT_t battery_status = {0};

GEO_DEST_RCHD_t GEO_DEST_RCHD = {0};
MOTOR_SPEED_t motor_speed = {0};


dbc_msg_hdr_t can_msg_hdr;
GEO_LOCATION_t geo_location = {0};
SENSOR_ULTRASONIC_t ultrasonic_sensor = {0};
GEO_COMPASS_t geo_compass_location = {0};
MASTER_SYSTEM_CMD_enum_E master_system_command;
MASTER_MOTOR_CMD_steer_E master_motor_command_steer;
MASTER_MOTOR_CMD_drive_E master_motor_command_drive;

const uint32_t                             MASTER_SYSTEM_CMD__MIA_MS = 1000;
const MASTER_SYSTEM_CMD_t                  MASTER_SYSTEM_CMD__MIA_MSG = {SYSTEM_STOP};
const uint32_t                             MASTER_MOTOR_CMD__MIA_MS= 1000;
const MASTER_MOTOR_CMD_t             MASTER_MOTOR_CMD__MIA_MSG = {};
const uint32_t                             MASTER_SYSTEM_STATUS__MIA_MS= 1000;
const MASTER_SYSTEM_STATUS_t               MASTER_SYSTEM_STATUS__MIA_MSG= {0};
const uint32_t                             SENSOR_ULTRASONIC__MIA_MS= 1000;
const SENSOR_ULTRASONIC_t                  SENSOR_ULTRASONIC__MIA_MSG= {0};
const uint32_t                             SENSOR_BATT__MIA_MS= 1000;
const SENSOR_BATT_t                        SENSOR_BATT__MIA_MSG= {0};

const uint32_t                             GEO_DEST_RCHD__MIA_MS= 1000;
const GEO_DEST_RCHD_t                      GEO_DEST_RCHD__MIA_MSG= {0};
const uint32_t                             GEO_LOCATION__MIA_MS= 1000;
const GEO_LOCATION_t                       GEO_LOCATION__MIA_MSG= {0};
const uint32_t                             GEO_COMPASS__MIA_MS= 1000;
const GEO_COMPASS_t                        GEO_COMPASS__MIA_MSG= {0};
const uint32_t                             MOTOR_SPEED__MIA_MS= 1000;
const MOTOR_SPEED_t                        MOTOR_SPEED__MIA_MSG= {0};

char lcd_cha;

Uart2 &U2 = Uart2::getInstance();


static GPIO front_left_pin(P1_28);
static GPIO front_right_pin(P1_30);
static GPIO reverse_pin(P0_30);
static GPIO back_left_pin(P0_29);
static GPIO back_right_pin(P2_4);
static GPIO brake_pin(P1_19);
static GPIO headlight_pin(P1_29);

static char received_ack;                         // received_acknowledge byte from LCD

bool destination_reached_flag;
bool system_command_flag;


void SEND_MSG_LCD(char a,char b,char c, char d,char e) {
	char character[6]={a,b,c,d,e,0};
	for(int i=0;i<5;i++) {
		character[5]^=character[i];
	}

	for(int i=0;i<6;i++) {
		U2.putChar(character[i],1);
	}
	U2.getChar(&received_ack,1);
}


void SEND_MSG_LCD_LAT(char a,char b,char c, char d,char e,char f,char g,char h, char i,char j,char k,char l) {
	char comm[13]={a,b,c,d,e,f,g,h,i,j,k,l,0};
	for(int x=0;x<12;x++) {
		comm[12]^=comm[x];
	}

	for(int y=0;y<13;y++) {
		U2.putChar(comm[y],1);
	}
	U2.getChar(&received_ack,1);

}

void SEND_MSG_LCD_LONG(char a,char b,char c, char d,char e,char f,char g,char h, char i,char j,char k,char l, char m,char n) {
	char comm[15]={a,b,c,d,e,f,g,h,i,j,k,l,m,n,0};
	for(int x=0;x<15;x++) {
		comm[14]^=comm[x];
	}

	for(int y=0;y<15;y++) {
		U2.putChar(comm[y],1);
	}
	U2.getChar(&received_ack,1);

}

void can_init_IO()
{
	CAN_init(can1, 100, 256, 256,0,0);

	CAN_bypass_filter_accept_all_msgs();

	CAN_reset_bus(can1);
}

void SEND_IO_HEARTBEAT()
{
	static int counter;
	counter++;


}

void system_destination_reached()
{
	static int counter;
	if(counter==2){
		front_left_pin.setLow();
		front_right_pin.setHigh();
		back_left_pin.setLow();
		back_right_pin.setHigh();
		headlight_pin.setHigh();
		reverse_pin.setHigh();
		brake_pin.setHigh();
	}
	if(counter==4){
		front_left_pin.setHigh();
		front_right_pin.setLow();
		back_left_pin.setHigh();
		back_right_pin.setLow();
		headlight_pin.setHigh();
		reverse_pin.setHigh();
		brake_pin.setHigh();
	}
	if(counter==6){
		front_left_pin.setHigh();
		front_right_pin.setHigh();
		back_left_pin.setHigh();
		back_right_pin.setHigh();
		headlight_pin.setLow();
		reverse_pin.setHigh();
		brake_pin.setHigh();
	}
	if(counter==8){
		front_left_pin.setHigh();
		front_right_pin.setHigh();
		back_left_pin.setHigh();
		back_right_pin.setHigh();
		headlight_pin.setHigh();
		reverse_pin.setLow();
		brake_pin.setHigh();
	}
	if(counter==10){
		front_left_pin.setHigh();
		front_right_pin.setHigh();
		back_left_pin.setHigh();
		back_right_pin.setHigh();
		headlight_pin.setHigh();
		reverse_pin.setHigh();
		brake_pin.setLow();

		counter=0;
	}
	counter++;

}
void RECEIVED_SYSTEM_CMD()
{

	master_system_command = system_cmd.MASTER_SYSTEM_CMD_enum;
	static int counter_sys_cmd;
	if(master_system_command == SYSTEM_START)
	{
		system_command_flag = true;
		SEND_MSG_LCD(ON,LED_VAL,0x08,0x00,ON);

		if(counter_sys_cmd %5 == 0)
		{
			SEND_MSG_LCD(ON,LED_VAL,0x06,0x00,0x00);
		}

	}
	counter_sys_cmd++;

	if(master_system_command == SYSTEM_STOP)
	{
		system_command_flag = false;
		SEND_MSG_LCD(ON,LED_VAL,0x06,OFF,ON);
		SEND_MSG_LCD(ON,LED_VAL,0x08,OFF,0x00);
		headlight_pin.setHigh();
		front_right_pin.setHigh();
		front_left_pin.setHigh();
		back_right_pin.setHigh();
		back_left_pin.setHigh();
		reverse_pin.setHigh();
		brake_pin.setHigh();

	}


	if(dbc_handle_mia_MASTER_SYSTEM_CMD(&system_cmd, 10))
	{
		SEND_MSG_LCD(ON,LED_VAL,0x06,OFF,0x00);

	}

}
void RECEIVED_SENSOR_ULTRASONIC()
{
	static int sen_count = 0;
	sen_count++;
	if(sen_count == 1)
	{
		if(ultrasonic_sensor.SENSOR_ULTRASONIC_right == 0)
			SEND_MSG_LCD(ON,GAUGE_VAL,0x02,OFF,0x32); // value = 50
		else
			SEND_MSG_LCD(ON,GAUGE_VAL,0x02,OFF,0x5A); // value = 90
	}
	else if(sen_count == 3)
	{
		if(ultrasonic_sensor.SENSOR_ULTRASONIC_left == 0)


			SEND_MSG_LCD(ON,GAUGE_VAL,0x03,OFF,0x32);

		else
			SEND_MSG_LCD(ON,GAUGE_VAL,0x03,OFF,0x5A);
	}

	else if(sen_count == 5)
	{
		if(ultrasonic_sensor.SENSOR_ULTRASONIC_middle == 0)

			SEND_MSG_LCD(ON,GAUGE_VAL,ON,OFF,0x32);

		else
			SEND_MSG_LCD(ON,GAUGE_VAL,ON,OFF,0x5A);

	}

	else if(sen_count == 7)
	{
		if(ultrasonic_sensor.SENSOR_ULTRASONIC_rear == 0)
		{
			SEND_MSG_LCD(ON,GAUGE_VAL,OFF,OFF,0x32);
			sen_count = 0;
		}
		else
			SEND_MSG_LCD(ON,GAUGE_VAL,OFF,OFF,0x5A);
		sen_count = 0;
	}

	if(dbc_handle_mia_SENSOR_ULTRASONIC(&ultrasonic_sensor, 10))
	{
		SEND_MSG_LCD(ON,GAUGE_VAL,OFF,OFF,0x00);
		SEND_MSG_LCD(ON,GAUGE_VAL,ON,OFF,0x00);
		SEND_MSG_LCD(ON,GAUGE_VAL,0x02,OFF,0x00);
		SEND_MSG_LCD(ON,GAUGE_VAL,0x03,OFF,0x00);
	}
}


void RECEIVED_MOTOR_CMD_LCD()
{
	static int motor_cmd_count;
	if(motor_cmd_count %3== 0)
	{
		switch( motor_cmd.MASTER_MOTOR_CMD_steer){
		case 0:
		case 1:
			SEND_MSG_LCD(ON,LED_VAL,0x02,OFF,ON); // direction left

			break;

		case 2:
		case 3:
			SEND_MSG_LCD(ON,LED_VAL,ON,OFF,ON);   // direction right
			break;

		case 4:
			SEND_MSG_LCD(ON,LED_VAL,0x04,OFF,ON); // direction front
			SEND_MSG_LCD(ON,LED_VAL,0x02,OFF,OFF);
			SEND_MSG_LCD(ON,LED_VAL,ON,OFF,OFF);
			SEND_MSG_LCD(ON,LED_VAL,0x03,OFF,OFF);
			break;
		case 5:
			SEND_MSG_LCD(ON,LED_VAL,0x03,OFF,ON);
			break;
		}
	}
	motor_cmd_count++;
}


void RECEIVED_MOTOR_CMD_LED()
{

	if(motor_cmd.MASTER_MOTOR_CMD_steer == 0)
	{
		brake_pin.setLow();
	}
	else
	{
		brake_pin.setHigh();
	}

	switch( motor_cmd.MASTER_MOTOR_CMD_steer){
	case 0:
	case 1:
		front_right_pin.setLow();
		back_right_pin.setLow();
		front_left_pin.setHigh();
		back_left_pin.setHigh();
		reverse_pin.setHigh();
		break;

	case 2:
	case 3:
		front_left_pin.setLow();
		back_left_pin.setLow();
		front_right_pin.setHigh();
		back_right_pin.setHigh();
		reverse_pin.setHigh();
		break;

	case 4:
		SEND_MSG_LCD(ON,LED_VAL,0x04,OFF,ON); // direction front
		front_left_pin.setHigh();
		front_right_pin.setHigh();
		back_left_pin.setHigh();
		back_right_pin.setHigh();
		break;

	case 5:
		reverse_pin.setLow();
		front_right_pin.setHigh();
		front_left_pin.setHigh();
		back_left_pin.setHigh();
		back_right_pin.setHigh();

		break;
	default:
		front_right_pin.setHigh();
		front_left_pin.setHigh();
		back_left_pin.setHigh();
		back_right_pin.setHigh();
		reverse_pin.setHigh();

	}

	if(dbc_handle_mia_MASTER_MOTOR_CMD(&motor_cmd, 10))
	{
		front_right_pin.setHigh();
		front_left_pin.setHigh();
		back_left_pin.setHigh();
		back_right_pin.setHigh();
		reverse_pin.setHigh();
	}

}


void RECEIVED_SYSTEM_STATUS()
{
	static int system_status_count;
	system_status.MASTER_SYSTEM_STATUS_io = 1;
	system_status.MASTER_SYSTEM_STATUS_motor = 1;
	if(system_status_count % 5 == 0)
	{
		SEND_MSG_LCD(ON,LED_VAL,BLE_SYSTEM_STATUS_VAL,OFF,system_status.MASTER_SYSTEM_STATUS_ble);
		SEND_MSG_LCD(ON,LED_VAL,GEO_SYSTEM_STATUS_VAL,OFF,system_status.MASTER_SYSTEM_STATUS_geo);
		SEND_MSG_LCD(ON,LED_VAL,IO_SYSTEM_STATUS_VAL,OFF,system_status.MASTER_SYSTEM_STATUS_io);
		SEND_MSG_LCD(ON,LED_VAL,MASTER_SYSTEM_STATUS_VAL,OFF,system_status.MASTER_SYSTEM_STATUS_master);
		SEND_MSG_LCD(ON,LED_VAL,MOTOR_SYSTEM_STATUS_VAL,OFF,system_status.MASTER_SYSTEM_STATUS_motor);
		SEND_MSG_LCD(ON,LED_VAL,SENSOR_SYSTEM_STATUS_VAL,OFF,system_status.MASTER_SYSTEM_STATUS_sensor);

	}
	system_status_count++;
}


void RECEIVED_SENSOR_BATTERY()
{
	SEND_MSG_LCD(ON,0x0B,0x04,OFF,battery_status.SENSOR_BATT_stat);

	if(dbc_handle_mia_SENSOR_BATT(&battery_status, 10))
	{
		SEND_MSG_LCD(ON,GAUGE_VAL,0x04,OFF,0x49);
	}

}



void RECEIVED_GEO_DEST_RCHD()
{


	if(GEO_DEST_RCHD.GEO_DEST_RCHD_stat == 1)
	{

		destination_reached_flag = true;
		system_destination_reached();
		SEND_MSG_LCD(0x01,0x0A,0x04,0x00,0x00);
	}
	else
	{
		destination_reached_flag = false;

	}

	if(dbc_handle_mia_GEO_DEST_RCHD(&GEO_DEST_RCHD, 10))
	{
		destination_reached_flag = false;
	}

}

void RECEIVED_GEO_LOCATION()
{
	float latitude = 0.0 , longitude = 0.0;
	char data_lat[15];
	char data_long[15];
	/*latitude = -121.333679;//geo_location.GEO_LOCATION_lat;
	longitude = 33.33334554;//geo_location.GEO_LOCATION_long;*/

	sprintf(data_lat, "%f", latitude);
	sprintf(data_long, "%f", longitude);

	/*SEND_MSG_LCD(0x02,ON,ON,data_lat[0],data_lat[1]);
	SEND_MSG_LCD(0x02,OFF,ON,data_long[0],data_long[1]);*/
	SEND_MSG_LCD_LAT(0x02,ON,ON,data_lat[0],data_lat[1],data_lat[2],data_lat[3],data_lat[4],data_lat[5],data_lat[6],data_lat[7],data_lat[8]);
	SEND_MSG_LCD_LONG(0x02,OFF,ON,data_long[0],data_long[1],data_long[2],data_long[3],data_long[4],data_long[5],data_long[5],data_long[6],data_long[7],data_long[8],data_long[9]);

}

void RECEIVED_MOTOR_SPEED()
{
	//printf("motor_speed   \n" );
	SEND_MSG_LCD(ON,0x08,OFF,OFF,motor_speed.MOTOR_SPEED_actual);

	if(dbc_handle_mia_MOTOR_SPEED(&motor_speed, 10))
	{
		SEND_MSG_LCD(ON,0x08,OFF,OFF,OFF);

	}

}
void RECEIVED_GEO_COMPASS_HEADER()
{

	static uint16_t magnetometer = geo_compass_location.GEO_COMPASS_mag;
	static uint8_t high_byte = 0, low_byte = 0;
	magnetometer = 279;
	if(geo_compass_location.GEO_COMPASS_mag < 255){
		low_byte |= magnetometer & 0xFF;
		SEND_MSG_LCD(ON,0x07,OFF,OFF,low_byte);
	}

	else{
		low_byte |= magnetometer & 0xFF;
		high_byte |= (magnetometer >> 8) & 0xFF;
		SEND_MSG_LCD(ON,0x07,OFF,low_byte,high_byte);
	}

	if(dbc_handle_mia_GEO_COMPASS(&geo_compass_location,10))
	{
		SEND_MSG_LCD(ON,0x07,OFF,OFF,magnetometer);
	}

}
void LCD_init(void) {
	U2.init(LCD_BAUD_RATE,LCD_RXQSIZE,LCD_TXQSIZE);
}
void IO_init()
{
	front_left_pin.setAsOutput();
	front_right_pin.setAsOutput();
	back_left_pin.setAsOutput();
	back_right_pin.setAsOutput();
	reverse_pin.setAsOutput();
	brake_pin.setAsOutput();
	headlight_pin.setAsOutput();


	front_left_pin.setHigh();
	front_right_pin.setHigh();
	back_left_pin.setHigh();
	back_right_pin.setHigh();
	reverse_pin.setHigh();
	brake_pin.setHigh();
	headlight_pin.setHigh();

	system_command_flag = false;

}


void start(){
	while(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;
		dbc_decode_MASTER_SYSTEM_CMD(&system_cmd, can_msg.data.bytes, &can_msg_hdr);
		dbc_decode_SENSOR_ULTRASONIC(&ultrasonic_sensor, can_msg.data.bytes, &can_msg_hdr);
		dbc_decode_MASTER_MOTOR_CMD(&motor_cmd, can_msg.data.bytes, &can_msg_hdr);
		dbc_decode_MASTER_SYSTEM_STATUS(&system_status, can_msg.data.bytes, &can_msg_hdr);
		dbc_decode_SENSOR_BATT(&battery_status, can_msg.data.bytes, &can_msg_hdr);
		dbc_decode_MOTOR_SPEED(&motor_speed, can_msg.data.bytes, &can_msg_hdr);
		dbc_decode_GEO_LOCATION(&geo_location, can_msg.data.bytes, &can_msg_hdr);
		dbc_decode_GEO_DEST_RCHD(&GEO_DEST_RCHD, can_msg.data.bytes, &can_msg_hdr);
	}

	RECEIVED_SYSTEM_CMD();
	system_command_flag = true;
	if(system_command_flag )
	{
		headlight_pin.setLow();
		RECEIVED_MOTOR_CMD_LED();
		RECEIVED_SENSOR_ULTRASONIC();
		RECEIVED_GEO_DEST_RCHD();
	}
}
void start_5sec(){
	RECEIVED_MOTOR_CMD_LCD();
	RECEIVED_MOTOR_SPEED();
}

void start_10sec()
{
	RECEIVED_SENSOR_BATTERY();
	RECEIVED_SYSTEM_STATUS();

}

void start_15sec()
{
	RECEIVED_GEO_LOCATION();
	RECEIVED_GEO_COMPASS_HEADER();
}
