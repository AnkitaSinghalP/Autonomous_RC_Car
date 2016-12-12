/*
 * current_sensor.cpp
 *
 *  Created on: Dec 6, 2016
 *      Author: BHUSHAN
 *      Description: This code is for measuring the voltage changes
 *      over time indicating the status of the car battery
 */
#include "current_sensor.hpp"
#include <stdlib.h>
#include <math.h>
#include "stdint.h"
#include "i2c_base.hpp"
#include <stdlib.h>
#include <math.h>
#include "stdint.h"
#include "i2c2.hpp"
#include "utilities.h"
#include "_can_dbc/generated_can.h"
#include "can.h"
#include "string.h"

int bus_voltage_V;
int battery_reading;
I2C2 &obj = I2C2::getInstance();
uint8_t inaAddress;
uint16_t ina219_calValue;
// The following multipliers are used to convert raw current and power
// values to mA and mW, taking into account the current config settings
float ina219_currentDivider_mA;
float ina219_powerDivider_mW;
SENSOR_BATT_t battery_status = {0};

uint16_t INA219_configure(uint8_t range, uint8_t gain, uint8_t busRes, uint8_t shuntRes, uint8_t mode)
{
    uint16_t config = 0;

    config |= (range << 13) + (gain << 11) + (busRes << 7) + (shuntRes << 3) + mode;
    INA219_writeRegister16(INA219_REG_CONFIG, config);
    return config;
}

void INA219_calibrate(uint16_t cal)
{

    ina219_calValue = cal;


    ina219_currentDivider_mA = 20.0;      // Current LSB = 50uA per bit (1000/50 = 20)
    ina219_powerDivider_mW = 1.0;         // Power LSB = 1mW per bit

    // Set Calibration register to 'Cal' calculated above
    INA219_writeRegister16(INA219_REG_CALIBRATION, ina219_calValue);

}

/**************************************************************************/
/*!
    @brief  Gets the raw bus voltage (16-bit signed integer, so +-32767)
 */
/**************************************************************************/
int16_t INA219_getBusVoltage_raw(void) {
    uint16_t value;
    value = INA219_readRegister16(INA219_REG_BUSVOLTAGE);

    // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
    return (int16_t)((value >> 3) * 4);
}

/**************************************************************************/
/*!
    @brief  Gets the raw shunt voltage (16-bit signed integer, so +-32767)
 */
/**************************************************************************/
int16_t INA219_getShuntVoltage_raw(void) {
    uint16_t value;
    value = INA219_readRegister16(INA219_REG_SHUNTVOLTAGE);
    return (int16_t)value;
}

/**************************************************************************/
/*!
    @brief  Gets the raw current value (16-bit signed integer, so +-32767)
 */
/**************************************************************************/
int16_t INA219_getCurrent_raw(void) {
    uint16_t value;

    // Sometimes a sharp load will reset the INA219, which will
    // reset the cal register, meaning CURRENT and POWER will
    // not be available ... avoid this by always setting a cal
    // value even if it's an unfortunate extra step
    INA219_writeRegister16(INA219_REG_CALIBRATION, ina219_calValue);

    // Now we can safely read the CURRENT register!
    value = INA219_readRegister16(INA219_REG_CURRENT);

    return (int16_t)value;
}

int16_t INA219_getPower_raw(void)
{
    uint16_t value;
    // Sometimes a sharp load will reset the INA219, which will
    // reset the cal register, meaning CURRENT and POWER will
    // not be available ... avoid this by always setting a cal
    // value even if it's an unfortunate extra step
    INA219_writeRegister16(INA219_REG_CALIBRATION, ina219_calValue);
    value = INA219_readRegister16(INA219_REG_POWER);

    return (int16_t) value;

}

float INA219_getPower_mW(void)
{
    int16_t value;
    value = INA219_getPower_raw();
    return (float)(value/ina219_powerDivider_mW);		//LSB =1mW
}



/**************************************************************************/
/*!
    @brief  Gets the shunt voltage in mV (so +-327mV)
 */
/**************************************************************************/
float INA219_getShuntVoltage_mV(void) {
    int16_t value;
    value = INA219_getShuntVoltage_raw();
    return value * 0.01;
}

/**************************************************************************/
/*!
    @brief  Gets the shunt voltage in volts
 */
/**************************************************************************/
float INA219_getBusVoltage_V(void) {
    int16_t value = INA219_getBusVoltage_raw();
    return value * 0.001;
}

/**************************************************************************/
/*!
    @brief  Gets the current value in mA, taking into account the
            config settings and current LSB
 */
/**************************************************************************/
float INA219_getCurrent_mA(void) {
    float valueDec = INA219_getCurrent_raw();
    valueDec /= ina219_currentDivider_mA;
    return valueDec;
}

int16_t INA219_readRegister16(uint8_t reg)
{
    int16_t value;
    static	uint8_t low_byte, hi_byte;
    obj.readReg(INA219_ADDRESS,reg);

    value = (hi_byte << 8) + low_byte;

    return value;
}

void INA219_writeRegister16(uint8_t reg, uint16_t val)
{
    uint8_t low_byte, hi_byte;
    low_byte = val&0x00FF;
    hi_byte = val>>8;
    obj.writeReg(INA219_ADDRESS,reg,hi_byte);
    delay_us(10);
    obj.writeReg(INA219_ADDRESS,reg,low_byte);
}
void current_sensor_measure(){

    bus_voltage_V = INA219_getBusVoltage_V();
    battery_reading = bus_voltage_V - battery_constant;
    battery_reading = (battery_reading / 3) * 100;
    battery_status.SENSOR_BATT_stat = battery_reading;
    dbc_encode_and_send_SENSOR_BATT(&battery_status);
}
