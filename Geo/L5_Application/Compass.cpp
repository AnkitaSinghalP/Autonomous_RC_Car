/*
 * Compass.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: Harsha
 */
#include "tasks.hpp"
#include "examples/examples.hpp"
#include "math.h"
#include "stdio.h"
#include "Compass.hpp"
#include "Compass_info.hpp"
#include "inttypes.h"
#include "io.hpp"
#include "can.h"
#include <stdint.h>
#include "i2c2.hpp"
#include "i2c_base.hpp"
#include "i2c2_device.hpp"
#include "tasks.hpp"
#include "utilities.h"

void CompassInit()
{
  //LPC_GPIO1->FIODIR &= ~(1<<9);
  LPC_I2C2->I2MASK0 = 0x1F;
  LPC_I2C2->I2CONSET = 0x40;
  LPC_SC->PCONP |= (1<<25);

  //Peripheral clock select
  LPC_SC->PCLKSEL1 &= ~(3<<18);
  LPC_SC->PCLKSEL1 |= (1<<18);

}



