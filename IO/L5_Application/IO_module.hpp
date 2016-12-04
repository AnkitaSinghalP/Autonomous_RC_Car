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
#endif /* L5_APPLICATION_SOURCE_IO_MODULE_HPP_ */
