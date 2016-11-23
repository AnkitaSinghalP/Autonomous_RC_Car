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
#include "uart2.hpp"
#include "uart3.hpp"

#define LCD_BAUD_RATE      115200
#define LCD_RXQSIZE        100
#define LCD_TXQSIZE        100

typedef enum{
    home_message,
    Geo_message,
    Sensors_message,
    Motor_message,
}LCD_SCREENS;


void SEND_IO_HEARTBEAT();
void can_init_IO();
void IO_init();
void RECEIVED_MOTOR_SPEED();
void RECEIVED_GEO_LOCATION();
void RECEIVED_GEO_DEST_RCHD();
void RECEIVED_BLE_MAP_DESTINATION_DATA();
void RECEIVED_BLE_MAP_START_DATA();
void RECEIVED_SENSOR_BATTERY();
void RECEIVED_SYSTEM_STATUS();
void RECEIVED_MOTOR_CMD();
void RECEIVED_SYSTEM_CMD();
void LCD_init(void);
void SEND_MSG_LCD(char a,char b,char c, char d,char e);
void lcd_receive();
/*
 * Printing on the lcd every 1000ms (1Hz), for now 100ms (10Hz)
 * Called by periodic scheduler
 */
void LCD_print();

/*
 * Sending any command to LCD
 * e.g.changing the screen on LCD, setting any value on a digital meter
 * A total of six bytes for one command, sixth byte if the checksum
 * Checksum is calculated in the function itself
 */
void SEND_MSG_LCD(char a,char b,char c, char d,char e);

/*
 * Sending a character string to LCD
 * array[] is the pointer to the input string
 * num is the size of the string
 */
void LCD_SEND_STRING(char array[],uint8_t num);

// lcd_test(uint8_t priority);

// bool init(void);
// bool run(void *p);

/*
 * Put the address of the global variable of a particular flag to get it set
 * Other all global variables of the same flag type will get reset
 */

void flag_change(bool* flagp);
void flag_page_change(bool* flagp);
void LCD_RECEIVE(void);





#endif /* L5_APPLICATION_SOURCE_IO_MODULE_HPP_ */
