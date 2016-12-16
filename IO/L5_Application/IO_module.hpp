/*
 * IO_module.hpp
 *
 *  Created on: Nov 19, 2016
 *      Author: BHUSHAN
 */

#ifndef L5_APPLICATION_SOURCE_IO_MODULE_HPP_
#define L5_APPLICATION_SOURCE_IO_MODULE_HPP_


#include "io.hpp"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "utilities.h"

#define LCD_BAUD_RATE      9600
#define LCD_RXQSIZE        1000
#define LCD_TXQSIZE        1000

#define BLE_SYSTEM_STATUS_VAL		0x0C
#define IO_SYSTEM_STATUS_VAL 		0x0B
#define MASTER_SYSTEM_STATUS_VAL 	0x05
#define GEO_SYSTEM_STATUS_VAL 		0x0D
#define MOTOR_SYSTEM_STATUS_VAL 	0x0A
#define SENSOR_SYSTEM_STATUS_VAL 	0x09
#define SCREEN_VAL 		0x0A

#define GAUGE_VAL 	0x0B
#define LED_VAL 	0x0E

#define ON 			0x01
#define OFF			0x00


typedef enum{
    home_message,
    Geo_message,
    Sensors_message,
    Motor_message,
}LCD_SCREENS;
void SEND_MSG_LCD(char a,char b,char c, char d,char e);
void start();
void RECEIVED_SYSTEM_CMD();
void can_init_IO();
void IO_init();
void LCD_init(void);
void SEND_IO_HEARTBEAT();
void system_destination_reached();
#endif /* L5_APPLICATION_SOURCE_IO_MODULE_HPP_ */
