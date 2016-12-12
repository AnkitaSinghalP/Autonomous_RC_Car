/*
 * navigation.h
 *
 *  Created on: Nov 15, 2016
 *      Author: Shaurya Jain & Harsha Gorantla
 */

#ifndef L5_APPLICATION_NAVIGATION_H_
#define L5_APPLICATION_NAVIGATION_H_

#include <uart2.hpp>
#include <string.h>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include "i2c2.hpp"
#include "i2c_base.hpp"
#include "i2c2_device.hpp"
#include "_can_dbc/generated_can.h"

extern GEO_DIRECTION_t geo_direction;

using namespace std;

#define GPS_BAUDRATE    115200
#define GPS_DATA_SIZE   255
#define RX_Q_SIZE       100
#define TX_Q_SIZE       1

#define STATIC_LAT      37
#define STATIC_LONG     121
#define RADIUS          20902231.64

#define MIN_ANGLE       0
#define MAX_ANGLE       360
#define DISTANCE_OFFSET 20
#define STEER_OFFSET    5
#define DEV_WR_ADDRESS  0x3C
#define DEV_RD_ADDRESS  0x3D
#define X_MSB_REG       0x03
#define X_LSB_REG       0x04
#define Y_MSB_REG       0x07
#define Y_LSB_REG       0x08
#define Z_MSB_REG       0x05
#define Z_LSB_REG       0x06
#define MODE_REG        0x02
#define CRA_REG_M       0x00
#define COMPASS_ADDR    0x1E
#define CASE1_MUL_CAL   0.8378
#define CASE2_MUL_CAL   1.1612
#define CASE3_MUL_CAL   1.25
#define CASE1_ADD_CAL   25
#define CASE2_ADD_CAL   -34.8387
#define CASE3_ADD_CAL   -425



typedef struct {
        float latitude;
        float longitude;
}gps_data;

/*enum direction {
	straight,
	half_left,
	full_left,
	half_right,
	full_right

    DIR_LEFT,
    DIR_HALF_LEFT,
    DIR_RIGHT,
    DIR_HALF_RIGHT,
    DIR_FORWARD

};*/

class Navigation
{
    public:
        void gps_init();
        void compass_init();
        bool geo();

        gps_data coordinates;
        gps_data next_checkpoint;
        uint16_t compass_angle;
        bool destination_reached;
        bool gps_fix;
        //direction steer;
        //GEO_DIRECTION_t steer;
        bool last_checkpoint_received;
        vector<gps_data> all_checkpoints;
        uint16_t gps_bearing_angle;

        float gps_distance;		//can be changed to a private variable after testing phase of the project


        Navigation(){
        	gps_fix = false;
            coordinates = {0};
            next_checkpoint = {0};
            gps_raw_data = new char[GPS_DATA_SIZE];
            gps_bearing_angle = -1;
            gps_distance = -1;
            compass_angle = -1;
            destination_reached = false;
            //steer = DIR_FORWARD;
            last_checkpoint_received = false;
            pop_next_checkpoint = true;
            next_checkpoint_reached = false;
        }

        ~Navigation(){
            delete [] gps_raw_data;
        }

    private:
        bool gps_get_raw_data();
        bool is_gpgga();
        bool gps_raw_data_checkSum(); //To check integrity of the raw gps data received upon uart
        bool parse_gps_raw_data();
        void gps_calculate_bearing_angle();
        void gps_calculate_distance();
        void compass_direction();
        void steer_command();
        void compass_calibrate();

        char* gps_raw_data;

        bool pop_next_checkpoint;
        bool next_checkpoint_reached;

};


#endif /* L5_APPLICATION_NAVIGATION_H_ */
