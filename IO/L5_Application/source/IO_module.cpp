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
#include "uart3.hpp"
#include "gpio.hpp"
#include "string.h"


MASTER_SYSTEM_CMD_t system_cmd;
MASTER_MOTOR_CMD_t motor_cmd = {};
MASTER_SYSTEM_STATUS_t system_status = {0};
SENSOR_BATT_t battery_status = {0};

GEO_DEST_RCHD_t GEO_DEST_RCHD = {0};
MOTOR_SPEED_t motor_speed = {0};
IO_HEARTBEAT_t io_heart_beat_msg = {1};
can_msg_t can_msg = { 0 };
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
const MASTER_MOTOR_CMD_t             MASTER_MOTOR_CMD__MIA_MSG = {STEER_STRAIGHT};
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


bool geo_flag = false;
bool geo_page_flag = false;
bool sensor_flag = false;
bool sensors_page_flag = false;
bool motor_flag = false;
bool motor_page_flag = false;
bool home_flag = false;
bool home_page_flag = false;
bool system_command_flag = false;
char lcd_cha;


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

static char received_ack;                         // received_acknowledge byte from LCD

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


void SEND_MSG_LCD_STRING(char a,char b,char c, char d,char e,char f,char g,char h, char i,char j,char k,char l, char m,char n) {
	char comm[15]={a,b,c,d,e,f,g,h,i,j,k,l,m,n,0};
	for(int i=0;i<15;i++) {
		comm[15]^=comm[i];
	}

	for(int i=0;i<16;i++) {
		U3.putChar(comm[i],10);
	}
	U3.getChar(&received_ack,10);

}


void can_init_IO()
{
	CAN_init(can1, 100, 128, 256,0,0);

	CAN_bypass_filter_accept_all_msgs();

	CAN_reset_bus(can1);
}


void SEND_IO_HEARTBEAT()
{

	dbc_encode_and_send_IO_HEARTBEAT(&io_heart_beat_msg);
}

void system_destination_reached()
{

	   static int counter;
	   if(counter==2){
	       left_pin.setLow();
	       right_pin.setHigh();
	       headlight_pin.setHigh();
	       reverse_pin.setHigh();
	       brake_pin.setHigh();
	   }
	   if(counter==4){
	       left_pin.setHigh();
	       right_pin.setLow();
	       headlight_pin.setHigh();
	       reverse_pin.setHigh();
	       brake_pin.setHigh();
	   }
	   if(counter==6){
	       left_pin.setHigh();
	       right_pin.setHigh();
	       headlight_pin.setLow();
	       reverse_pin.setHigh();
	       brake_pin.setHigh();
	       }
	   if(counter==8){
	       left_pin.setHigh();
	       right_pin.setHigh();
	       headlight_pin.setHigh();
	       reverse_pin.setLow();
	       brake_pin.setHigh();
	       }
	   if(counter==10){
	       left_pin.setHigh();
	       right_pin.setHigh();
	       headlight_pin.setHigh();
	       reverse_pin.setHigh();
	       brake_pin.setLow();

	       counter=0;
	   }
	   counter++;

}
void RECEIVED_SYSTEM_CMD()
{
	/*	while(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;


	}*/
	master_system_command = system_cmd.MASTER_SYSTEM_CMD_enum;
	//printf("system message  %d \n",master_system_command );
	if(master_system_command == SYSTEM_START)
	{
		system_command_flag = true;
		SEND_MSG_LCD(ON,LED_VAL,0x08,OFF,ON);
		system_start_pin.setLow();
		//return true;

	}
	if(master_system_command == SYSTEM_STOP)
	{
		system_command_flag = false;
		SEND_MSG_LCD(ON,LED_VAL,0x06,OFF,ON);
		headlight_pin.setHigh();
		system_stop_pin.setLow();
		right_pin.setHigh();
		left_pin.setHigh();
		reverse_pin.setHigh();
		//return false;

	}
	/*if(master_system_command == SYSTEM_RESET)
	{
		system_command_flag = true;
		SEND_MSG_LCD(ON,LED_VAL,0x07,OFF,ON);
		system_reset_pin.setLow();
		//return true;
	}*/


	//}

	if(dbc_handle_mia_MASTER_SYSTEM_CMD(&system_cmd, 10))
	{
		SEND_MSG_LCD(ON,LED_VAL,OFF,OFF,ON);
		//return true;
	}

}
void RECEIVED_SENSOR_ULTRASONIC()
{
	/*
	if(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;
	 */

	//SEND_MSG_LCD(ON,MOTOR_SYSTEM_STATUS_VAL,ON,OFF,OFF);
	//printf("ultrasonic sensor  \n" );

	if(ultrasonic_sensor.SENSOR_ULTRASONIC_right == 0)
	{SEND_MSG_LCD(ON,GAUGE_VAL,0x02,OFF,0x32);} // value = 50
	if(ultrasonic_sensor.SENSOR_ULTRASONIC_right == 1)
	{SEND_MSG_LCD(ON,IO_SYSTEM_STATUS_VAL,0x02,OFF,0x5A);} // value = 90

	if(ultrasonic_sensor.SENSOR_ULTRASONIC_left == 0)
	{SEND_MSG_LCD(ON,GAUGE_VAL,0x03,OFF,0x32);}
	if(ultrasonic_sensor.SENSOR_ULTRASONIC_left == 1)
	{SEND_MSG_LCD(ON,GAUGE_VAL,0x03,OFF,0x5A);}


	if(ultrasonic_sensor.SENSOR_ULTRASONIC_middle == 0)
	{SEND_MSG_LCD(ON,GAUGE_VAL,ON,OFF,0x32);}
	if(ultrasonic_sensor.SENSOR_ULTRASONIC_middle == 1)
	{SEND_MSG_LCD(ON,GAUGE_VAL,ON,OFF,0x5A);}

	if(ultrasonic_sensor.SENSOR_ULTRASONIC_rear == 0)
	{SEND_MSG_LCD(ON,GAUGE_VAL,OFF,OFF,0x32);}
	if(ultrasonic_sensor.SENSOR_ULTRASONIC_rear == 1)
	{	SEND_MSG_LCD(ON,GAUGE_VAL,OFF,OFF,0x5A);}

	//}
	if(dbc_handle_mia_SENSOR_ULTRASONIC(&ultrasonic_sensor, 10))
	{
		SEND_MSG_LCD(ON,GAUGE_VAL,OFF,OFF,0x32);
		SEND_MSG_LCD(ON,GAUGE_VAL,ON,OFF,0x32);
		SEND_MSG_LCD(ON,GAUGE_VAL,0x02,OFF,0x32);
		SEND_MSG_LCD(ON,GAUGE_VAL,0x03,OFF,0x32);
	}
}


void RECEIVED_MOTOR_CMD()
{
	/*if(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;*/

	//printf("motor command   %d \n",motor_cmd.MASTER_MOTOR_CMD_steer );
	switch( motor_cmd.MASTER_MOTOR_CMD_steer){
	case 0:
	case 1:
		left_pin.setLow();
		right_pin.setHigh();
		reverse_pin.setHigh();
		SEND_MSG_LCD(ON,LED_VAL,0x02,OFF,ON); // direction left


		SEND_MSG_LCD(ON,LED_VAL,ON,OFF,OFF);
		SEND_MSG_LCD(ON,LED_VAL,0x03,OFF,OFF);
		SEND_MSG_LCD(ON,LED_VAL,0x04,OFF,OFF);

		break;

	case 2:
	case 3:
		right_pin.setLow();
		left_pin.setHigh();
		reverse_pin.setHigh();
		SEND_MSG_LCD(ON,LED_VAL,ON,OFF,ON);   // direction right

		SEND_MSG_LCD(ON,LED_VAL,0x02,OFF,OFF);
		SEND_MSG_LCD(ON,LED_VAL,0x04,OFF,OFF);
		SEND_MSG_LCD(ON,LED_VAL,0x03,OFF,OFF);
		break;

	case 4:
		SEND_MSG_LCD(ON,LED_VAL,0x04,OFF,ON); // direction front
		left_pin.setHigh();
		right_pin.setHigh();
		SEND_MSG_LCD(ON,LED_VAL,0x02,OFF,OFF);
		SEND_MSG_LCD(ON,LED_VAL,ON,OFF,OFF);
		SEND_MSG_LCD(ON,LED_VAL,0x03,OFF,OFF);
		break;
	case 5:
		reverse_pin.setLow();
		right_pin.setHigh();
		left_pin.setHigh();
		SEND_MSG_LCD(ON,LED_VAL,0x03,OFF,ON);
		break;
	default:
		right_pin.setHigh();
		left_pin.setHigh();
		reverse_pin.setHigh();
		SEND_MSG_LCD(ON,LED_VAL,0x02,OFF,OFF);
		SEND_MSG_LCD(ON,LED_VAL,ON,OFF,OFF);
		SEND_MSG_LCD(ON,LED_VAL,0x03,OFF,OFF);
		SEND_MSG_LCD(ON,LED_VAL,0x04,OFF,OFF);
	}
	//}
	/*right_pin.setHigh();
	left_pin.setHigh();
	reverse_pin.setHigh();*/



	if(dbc_handle_mia_MASTER_MOTOR_CMD(&motor_cmd, 10))
	{
		right_pin.setHigh();
		left_pin.setHigh();
		reverse_pin.setHigh();
		SEND_MSG_LCD(ON,LED_VAL,0x02,OFF,OFF);
		SEND_MSG_LCD(ON,LED_VAL,ON,OFF,OFF);
		SEND_MSG_LCD(ON,LED_VAL,0x03,OFF,OFF);
		SEND_MSG_LCD(ON,LED_VAL,0x04,OFF,OFF);
	}

}

void RECEIVED_SYSTEM_STATUS()
{
	/*
	if(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;
	 */
	//printf("%f",system_status.MASTER_SYSTEM_STATUS_util);
	//}
	//SEND_MSG_LCD(ON,MOTOR_SYSTEM_STATUS_VAL,0x02,OFF,OFF);

	if(system_status.MASTER_SYSTEM_STATUS_ble == 1)
	{
		SEND_MSG_LCD(ON,LED_VAL,BLE_SYSTEM_STATUS_VAL,OFF,ON);

	}
	if(system_status.MASTER_SYSTEM_STATUS_geo == 1)
	{
		SEND_MSG_LCD(ON,LED_VAL,GEO_SYSTEM_STATUS_VAL,OFF,ON);
	}
	if(system_status.MASTER_SYSTEM_STATUS_io == 1)
	{
		SEND_MSG_LCD(ON,LED_VAL,IO_SYSTEM_STATUS_VAL,OFF,ON);
	}
	if(system_status.MASTER_SYSTEM_STATUS_master == 1)
	{
		SEND_MSG_LCD(ON,LED_VAL,MASTER_SYSTEM_STATUS_VAL,OFF,ON);
	}
	if(system_status.MASTER_SYSTEM_STATUS_motor == 1)
	{
		SEND_MSG_LCD(ON,LED_VAL,MOTOR_SYSTEM_STATUS_VAL,OFF,ON);
	}
	if(system_status.MASTER_SYSTEM_STATUS_sensor == 1)
	{
		SEND_MSG_LCD(ON,LED_VAL,SENSOR_SYSTEM_STATUS_VAL,OFF,ON);
	}


	if(system_status.MASTER_SYSTEM_STATUS_ble == 0)
	{
		SEND_MSG_LCD(ON,LED_VAL,BLE_SYSTEM_STATUS_VAL,OFF,OFF);

	}
	if(system_status.MASTER_SYSTEM_STATUS_geo == 0)
	{
		SEND_MSG_LCD(ON,LED_VAL,GEO_SYSTEM_STATUS_VAL,OFF,OFF);
	}
	if(system_status.MASTER_SYSTEM_STATUS_io == 0)
	{
		SEND_MSG_LCD(ON,LED_VAL,IO_SYSTEM_STATUS_VAL,OFF,OFF);
	}
	if(system_status.MASTER_SYSTEM_STATUS_master == 0)
	{
		SEND_MSG_LCD(ON,LED_VAL,MASTER_SYSTEM_STATUS_VAL,OFF,OFF);
	}
	if(system_status.MASTER_SYSTEM_STATUS_motor == 0)
	{
		SEND_MSG_LCD(ON,LED_VAL,MOTOR_SYSTEM_STATUS_VAL,OFF,OFF);
	}
	if(system_status.MASTER_SYSTEM_STATUS_sensor == 0)
	{
		SEND_MSG_LCD(ON,LED_VAL,SENSOR_SYSTEM_STATUS_VAL,OFF,OFF);
	}



	if(dbc_handle_mia_MASTER_SYSTEM_STATUS(&system_status, 10))
	{
		SEND_MSG_LCD(ON,LED_VAL,BLE_SYSTEM_STATUS_VAL,OFF,OFF);
		SEND_MSG_LCD(ON,LED_VAL,GEO_SYSTEM_STATUS_VAL,OFF,OFF);
		SEND_MSG_LCD(ON,LED_VAL,IO_SYSTEM_STATUS_VAL,OFF,OFF);
		SEND_MSG_LCD(ON,LED_VAL,MASTER_SYSTEM_STATUS_VAL,OFF,OFF);
		SEND_MSG_LCD(ON,LED_VAL,MOTOR_SYSTEM_STATUS_VAL,OFF,OFF);
		SEND_MSG_LCD(ON,LED_VAL,SENSOR_SYSTEM_STATUS_VAL,OFF,OFF);
	}

}


void RECEIVED_SENSOR_BATTERY()
{
	/*if(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;*/
	//}


	SEND_MSG_LCD(ON,0x0B,0x04,OFF,battery_status.SENSOR_BATT_stat);

	if(dbc_handle_mia_SENSOR_BATT(&battery_status, 10))
	{
		SEND_MSG_LCD(ON,GAUGE_VAL,0x04,OFF,OFF);
	}

}



void RECEIVED_GEO_DEST_RCHD()
{


	if(GEO_DEST_RCHD.GEO_DEST_RCHD_stat == 1)
	{
		SEND_MSG_LCD(ON,0x0A,0x04,OFF,OFF);
		system_destination_reached();
	}
	else
	{

	}
	if(dbc_handle_mia_GEO_DEST_RCHD(&GEO_DEST_RCHD, 10))
	{
		GEO_DEST_RCHD.GEO_DEST_RCHD_stat = 0;
	}

}

void RECEIVED_GEO_LOCATION()
{
	float latitude = 0.0 , longitude = 0.0;

	latitude = geo_location.GEO_LOCATION_lat;
	longitude = geo_location.GEO_LOCATION_long;

	SEND_MSG_LCD(0x02,ON,ON,latitude,'.');
	SEND_MSG_LCD(0x02,ON,ON,longitude,'.');

	if(dbc_handle_mia_GEO_LOCATION(&geo_location, 10))
	{

	}

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

	uint16_t magnetometer = geo_compass_location.GEO_COMPASS_mag;
	static uint8_t high_byte = 0, low_byte = 0;

	if(geo_compass_location.GEO_COMPASS_mag < 255)
		SEND_MSG_LCD(ON,0x07,OFF,OFF,magnetometer);

	else{
		low_byte |= magnetometer;
		high_byte |= (magnetometer >> 8);
		SEND_MSG_LCD(ON,0x07,OFF,low_byte,high_byte);
	}

	if(dbc_handle_mia_MOTOR_SPEED(&motor_speed, 10))
	{
		SEND_MSG_LCD(ON,0x07,OFF,OFF,magnetometer);
	}

}
void LCD_init(void) {
	U3.init(LCD_BAUD_RATE,LCD_RXQSIZE,LCD_TXQSIZE);
	SEND_MSG_LCD(ON,MOTOR_SYSTEM_STATUS_VAL,ON,OFF,OFF);
	//SEND_MSG_LCD(ON,MOTOR_SYSTEM_STATUS_VAL,OFF,OFF,OFF);

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

	left_pin.setHigh();
	right_pin.setHigh();
	reverse_pin.setHigh();
	start_pin.setHigh();
	stop_pin.setHigh();
	brake_pin.setHigh();
	resume_pin.setHigh();
	headlight_pin.setHigh();
	system_start_pin.setHigh();
	system_stop_pin.setHigh();
	system_reset_pin.setHigh();
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

	if(system_command_flag){
		headlight_pin.setLow();

		//if( can_msg.msg_id == 211)
		RECEIVED_SENSOR_ULTRASONIC();

		//if(can_msg.msg_id== 151)
		RECEIVED_MOTOR_CMD();

		//if(can_msg.msg_id ==  162)
		RECEIVED_SYSTEM_STATUS();

		//if( can_msg.msg_id == 213)
		RECEIVED_SENSOR_BATTERY();


		//if(can_msg.msg_id ==  561)
		RECEIVED_MOTOR_SPEED();

		//if(can_msg.msg_id == 421)
		RECEIVED_GEO_LOCATION();

		//if(can_msg.msg_id == 413)
		RECEIVED_GEO_DEST_RCHD();

		//if(can_msg.msg_id== 461)
		RECEIVED_GEO_COMPASS_HEADER();
	}
}
