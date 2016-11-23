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
#include "uart2.hpp"

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

const uint32_t             SYSTEM_CMD__MIA_MS = 1000;
const MASTER_SYSTEM_CMD_t         SYSTEM_CMD__MIA_MSG = {SYSTEM_STOP};
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
Uart2 &U2 = Uart2::getInstance();


char Bytes_to_send[6]={0};                 // 6 bytes received from the LCD
char received_ack=0;                         // received_acknowledge byte from LCD

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



bool period_init(void)
{
	//u3.init(38400,100,100);
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

		dbc_decode_MASTER_SYSTEM_CMD(&system_cmd, can_msg.data.bytes, &can_msg_hdr);

	}


	if(dbc_handle_mia_MASTER_SYSTEM_CMD(&system_cmd, 10))
	{

	}

}

void RECEIVED_MOTOR_CMD()
{
	while(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_MASTER_MOTOR_CMD(&motor_cmd, can_msg.data.bytes, &can_msg_hdr);

	}


	if(dbc_handle_mia_MASTER_MOTOR_CMD(&motor_cmd, 10))
	{

	}

}

void RECEIVED_SYSTEM_STATUS()
{
	while(CAN_rx(can1, &can_msg, 0))
	{
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_MASTER_SYSTEM_STATUS(&system_status, can_msg.data.bytes, &can_msg_hdr);

	}


	if(dbc_handle_mia_MASTER_SYSTEM_STATUS(&system_status, 10))
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

void LCD_init(void) {
    U2.init(LCD_BAUD_RATE,LCD_RXQSIZE,LCD_TXQSIZE);
    dc_slow = 6.35;
    dc_normal = 6.28;
    dc_turbo = 6.20;
}

void SEND_MSG_LCD(char a,char b,char c, char d,char e) {
    char comm[6]={a,b,c,d,e,0};
    for(int i=0;i<5;i++) {
        comm[5]^=comm[i];
    }

    for(int i=0;i<6;i++) {
        U2.putChar(comm[i],10);
    }
    U2.getChar(&received_ack,10);
}

void put_string(char array[], uint8_t object_no, uint8_t num){
    uint8_t checksum=0x02^object_no^num;
    U2.putChar(0x02,10);
    U2.putChar(object_no,10);
    U2.putChar(num,10);

    for(int i=0;i<num;i++){
        U2.putChar(array[i],10);
        checksum^=array[i];
    }

    U2.putChar(checksum,10);
   U2.getChar(&received_ack,10);
}


void lcd_print(){

     static bool headlights_on = false;
     bool flag_headlight = false;

     //if((sensor_lcd.data.bytes[0]!=0)||(sensor_lcd.data.bytes[1]!=0)||(sensor_lcd.data.bytes[2]!=0)){
     SEND_MSG_LCD(0x01,0x16,0x00,0x00,0x00);


     if(Bytes_to_send[1]==0x06 && Bytes_to_send[2]==0x03) {
         lcdscreen=home_message;
         flag_change(&geo_flag);
     }
     else if(Bytes_to_send[1]==0x06 && Bytes_to_send[2]==0x00) {
         lcdscreen=Sensors_message;
         flag_change(&sensor_flag);
     }
     else if(Bytes_to_send[1]==0x06 && Bytes_to_send[2]==0x01) {
          lcdscreen=Motor_message;
          flag_change(&motor_flag);
     }
     else if(Bytes_to_send[1]==0x1e && (Bytes_to_send[2]==0x02 || Bytes_to_send[2]==0x04 || Bytes_to_send[2]==0x01)) {
          lcdscreen=home_message;
          flag_change(&home_flag);
     }
     if(geo_flag && !geo_page_flag) {
         SEND_MSG_LCD(0x01,0x0a,0x02,0x00,0x00); // switch to form2
         flag_page_change(&geo_page_flag);
     }
     else if(sensor_flag && !sensors_page_flag) {
         SEND_MSG_LCD(0x01,0x0a,0x03,0x00,0x00); // switch to form3
         flag_page_change(&sensors_page_flag);
     }
     else if(motor_flag && !motor_page_flag) {
          SEND_MSG_LCD(0x01,0x0a,0x04,0x00,0x00); // switch to form4
          flag_page_change(&motor_page_flag);
      }
     else if(home_flag && !home_page_flag) {
          SEND_MSG_LCD(0x01,0x0a,0x00,0x00,0x00); // switch to form0
          flag_page_change(&home_page_flag);
      }
/*
     if(lcdscreen == Geo){
         SEND_MSG_LCD(0x01, 0x07, 0x01, 0x00, geo_msg.GEO_ANGLE_heading_cmd);
         static char string[30]={0};
         sprintf(string,"%f", geo_loc_msg.GEO_LOC_LAT_cmd);
         put_string(string, 0,(uint8_t)strlen(string));
         sprintf(string,"%f", geo_loc_msg.GEO_LOC_LONG_cmd);
         put_string(string, 1,(uint8_t)strlen(string));
     }
     else if(lcdscreen == Sensors){
         printf("left:%d\n",sensor_msg.SENSOR_SONARS_front_left);
         printf("right:%d\n",sensor_msg.SENSOR_SONARS_front_right);
         printf("center:%d\n",sensor_msg.SENSOR_SONARS_front_center);
         //printf("ds_center:%d\n",dist_sensor_can_msg.data.bytes[0]);
         SEND_MSG_LCD(0x01, 0x0b, 0x00, 0x00, sensor_msg.SENSOR_SONARS_front_center);
         SEND_MSG_LCD(0x01, 0x0b, 0x02, 0x00, sensor_msg.SENSOR_SONARS_front_left);
         SEND_MSG_LCD(0x01, 0x0b, 0x01, 0x00, sensor_msg.SENSOR_SONARS_front_right);
         SEND_MSG_LCD(0x01, 0x0b, 0x04, 0x00, sensor_msg.SENSOR_SONARS_left);
         SEND_MSG_LCD(0x01, 0x0b, 0x03, 0x00, sensor_msg.SENSOR_SONARS_right);
         SEND_MSG_LCD(0x01, 0x0b, 0x05, 0x00, sensor_msg.SENSOR_SONARS_breceived_ack);
     }
     else if(lcdscreen == home){
         SEND_MSG_LCD(0x01, 0x1a, 0x00, 0x00, sensor_bat_msg.SENSOR_BAT_cmd); // setting the battery meter here
         if(Bytes_to_send[1]==0x21 && Bytes_to_send[2]==0x00){
             headlights_on = headlights_on? false:true;
             if(headlights_on)
                 SEND_MSG_LCD(0x01, 0x13, 0x00, 0x00, 0x01);
             else
                 SEND_MSG_LCD(0x01, 0x13, 0x00, 0x00, 0x00);
             Bytes_to_send[1] = 0x00;
         }
     }
     else if(lcdscreen == Motor){
         //printf("speed:%d\n",motor_msg.MOTORIO_DIRECTION_speed_cmd);
         //printf("turn:%d\n",motor_msg.MOTORIO_DIRECTION_turn_cmd);
         SEND_MSG_LCD(0x01, 0x0b, 0x06, 0x00, motor_msg.MOTORIO_DIRECTION_speed_cmd);
         SEND_MSG_LCD(0x01, 0x10, 0x00, 0x00, motor_msg.MOTORIO_DIRECTION_turn_cmd);
         if(motor_msg.MOTORIO_DIRECTION_turn_cmd==breceived_ack)
             SEND_MSG_LCD(0x01, 0x0b, 0x07, 0x00, motor_msg.MOTORIO_DIRECTION_turn_cmd);
         else
             SEND_MSG_LCD(0x01, 0x10, 0x00, 0x00, 0);
     }*/
     if(lcdscreen == Geo_message){

          /*geo_loc* geo_loc_Ptr = (geo_loc*)(&geo_loc_lcd.data);
          geo_spd_angle* gsaPtr = (geo_spd_angle*)(&geo_lcd.data);
          //printf("lat:%f\n",(float)geo_loc_data->latitude);
          //printf("long:%f\n",(float)geo_loc_data->longitude);
          SEND_MSG_LCD(0x01, 0x07, 0x01, geo_lcd.data.bytes[2], geo_lcd.data.bytes[1]);
          static char string[30]={0};

          sprintf(string,"%f", (float)geo_loc_Ptr->latitude);
          put_string(string, 0,(uint8_t)strlen(string));                //Print Latitude on LCD
          sprintf(string,"%f", (float)geo_loc_Ptr->longitude);
          put_string(string, 1,(uint8_t)strlen(string));                //Print Longitude on LCD
          sprintf(string,"%i", gsaPtr->bearing);
          put_string(string, 2,(uint8_t)strlen(string));                //Print desired Bearing of car on LCD
          sprintf(string,"%f", (float)(gsaPtr->distance)/10000);
          put_string(string, 3,(uint8_t)strlen(string));        */        //Print Car's Distance from next checkpoint on LCD
      }
      else if(lcdscreen == Sensors_message){
          //printf("left:%d\n",sensor_lcd.data.bytes[0]);
          //printf("right:%d\n",sensor_lcd.data.bytes[1]);
          //printf("center:%d\n",sensor_lcd.data.bytes[2]);
          //printf("ds_center:%d\n",dist_sensor_can_msg.data.bytes[0]);
         /* SEND_MSG_LCD(0x01, 0x0b, 0x00, 0x00, sensor_lcd.data.bytes[2]);       //Print Front_Center sensor data on LCD
          SEND_MSG_LCD(0x01, 0x0b, 0x02, 0x00, sensor_lcd.data.bytes[0]);       //Print Front_Left sensor data on LCD
          SEND_MSG_LCD(0x01, 0x0b, 0x01, 0x00, sensor_lcd.data.bytes[1]);       //Print Front_Right sensor data on LCD
          SEND_MSG_LCD(0x01, 0x0b, 0x04, 0x00, sensor_lcd.data.bytes[3]);       //Print Left sensor data on LCD
          SEND_MSG_LCD(0x01, 0x0b, 0x03, 0x00, sensor_lcd.data.bytes[4]);       //Print Right sensor data on LCD
          SEND_MSG_LCD(0x01, 0x0b, 0x05, 0x00, sensor_lcd.data.bytes[5]);       //Print Breceived_ack sensor data on LCD*/
      }
      else if(lcdscreen == home_message){
         // SEND_MSG_LCD(0x01, 0x1a, 0x00, 0x00, sensor_bat_msg.SENSOR_BAT_cmd); // setting the battery meter here
          if(Bytes_to_send[1]==0x21 && Bytes_to_send[2]==0x01){
              headlights_on = headlights_on? false:true;
              if(headlights_on)
            	  SEND_MSG_LCD(0x01, 0x13, 0x01, 0x00, 0x01);
              else
            	  SEND_MSG_LCD(0x01, 0x13, 0x01, 0x00, 0x00);
              Bytes_to_send[1] = 0x00;
          }
      }
      else if(lcdscreen == Motor_message){
          //printf("speed:%d\n",motor_msg.MOTORIO_DIRECTION_speed_cmd);
          //printf("turn:%d\n",motor_msg.MOTORIO_DIRECTION_turn_cmd);
          if(Bytes_to_send[1]==0x21 && Bytes_to_send[2]==0x00)
              dc_slow = dc_slow-0.01;
          else if(Bytes_to_send[1]==0x21 && Bytes_to_send[2]==0x04)
              dc_slow = dc_slow+0.01;
          else if(Bytes_to_send[1]==0x21 && Bytes_to_send[2]==0x02)
              dc_normal =dc_normal-0.01;
          else if(Bytes_to_send[1]==0x21 && Bytes_to_send[2]==0x05)
              dc_normal = dc_normal+0.01;
          else if(Bytes_to_send[1]==0x21 && Bytes_to_send[2]==0x03)
              dc_turbo = dc_turbo-0.01;
          else if(Bytes_to_send[1]==0x21 && Bytes_to_send[2]==0x06)
              dc_turbo = dc_turbo+0.01;
          Bytes_to_send[1] = 0x00; // resetting the value so that the if/else conditions r not entered again and again

          static char string[30]={0};
          sprintf(string,"%.2f", dc_slow);
          put_string(string, 4,(uint8_t)strlen(string));                //Print Latitude on LCD
          sprintf(string,"%.2f", dc_normal);
          put_string(string, 5,(uint8_t)strlen(string));                //Print Longitude on LCD
          sprintf(string,"%.2f", dc_turbo);
          put_string(string, 6,(uint8_t)strlen(string));
          /*
          SEND_MSG_LCD(0x01, 0x0b, 0x06, 0x00, motor_lcd.data.bytes[0]);        //Print Motor Speed data on LCD
          SEND_MSG_LCD(0x01, 0x10, 0x00, 0x00, motor_lcd.data.bytes[1]);        //Print Motor turn data on LCD
         if(motor_msg.MOTORIO_DIRECTION_turn_cmd==breceived_ack)
              SEND_MSG_LCD(0x01, 0x0b, 0x07, 0x00, motor_lcd.data.bytes[1]);
          else
              SEND_MSG_LCD(0x01, 0x10, 0x00, 0x00, 0);*/
      }

}


void lcd_receive(){

      U2.getChar(&lcd_cha,0);
      if (lcd_cha == 0x07)
      {
          Bytes_to_send[0]=lcd_cha;
          for(int i=1;i<6;i++) {
              U2.getChar(Bytes_to_send+i,0);
          }
      }
}
