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

void start();
bool RECEIVED_SYSTEM_CMD();
void can_init_IO();
void IO_init();
void LCD_init(void);
#endif /* L5_APPLICATION_SOURCE_IO_MODULE_HPP_ */
