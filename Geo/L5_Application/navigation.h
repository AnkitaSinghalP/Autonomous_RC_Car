/*
 * navigation.h
 *
 *  Created on: Nov 15, 2016
 *      Author: shaur
 */

#ifndef L5_APPLICATION_NAVIGATION_H_
#define L5_APPLICATION_NAVIGATION_H_

#include <uart2.hpp>
#include <string.h>
#include <math.h>
#include <cstdlib>
#include <stdio.h>

/*
 * Configuration of GPS Module
 * Adafruit Ultimate GPS breakout board
 *
 */
/*
#define SET_OUTPUT_RMC 		"$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29<CR><LF>"
#define SET_OUTPUT_RMC_GGA 	"$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28<CR><LF>"
#define SET_OUTPUT_ALLDATA 	"$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28<CR><LF>"
#define SET_OUTPUT_OFF 		"$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28<CR><LF>"

#define UPDATE_RATE_10HZ 	"$PMTK220,100*2F<CR><LF>"

#define BAUD_RATE_38400 	"$PMTK251,38400*27<CR><LF>"
*/

#define GPS_BAUDRATE 	115200
#define GPS_DATA_SIZE	255
#define RX_Q_SIZE		255
#define TX_Q_SIZE		255

#define STATIC_LAT		37
#define STATIC_LONG		121

typedef struct {
		float latitude;
		float longitude;
}gps_data;


class Navigation
{
	public:
		void gps_init();
		bool geo();

	private:
		void gps_get_raw_data();
		bool is_gpgga();
		bool gps_raw_data_checkSum(); //To check integrity of the raw gps data received on uart
		bool parse_gps_raw_data();

		char* gps_raw_data;
		gps_data coordinates;

};


#endif /* L5_APPLICATION_NAVIGATION_H_ */
