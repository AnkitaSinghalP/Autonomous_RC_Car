/*
 * gps.hpp
 *
 *  Created on: Oct 13, 2016
 *      Author: shaurya
 */

#include <string.h>
#include <cstdlib>


#ifndef L5_APPLICATION_GPS_HPP_
#define L5_APPLICATION_GPS_HPP_


#define SET_OUTPUT_RMC 		"$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29<CR><LF>"
#define SET_OUTPUT_RMC_GGA 	"$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28<CR><LF>"
#define SET_OUTPUT_ALLDATA 	"$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28<CR><LF>"
#define SET_OUTPUT_OFF 		"$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28<CR><LF>"

#define UPDATE_RATE_10HZ 	"$PMTK220,100*2F<CR><LF>"

#define BAUD_RATE_38400 	"$PMTK251,38400*27<CR><LF>"


int32_t latitude_fixed, longitude_fixed;
float latitude, longitude;
float latitudeDegrees, longitudeDegrees;
int static_lat = 37,static_long=121;
bool fix;

uint8_t parseHex(char c)
{
    if (c < '0')
      return 0;

    if (c <= '9')
      return c - '0';

    if (c < 'A')
       return 0;

    if (c <= 'F')
       return (c - 'A')+10;

    return 0;
}

bool checkSum(char *nmea)
{
	if (nmea[strlen(nmea)-4] == '*')
	{
		uint16_t sum = parseHex(nmea[strlen(nmea)-3]) * 16;
		sum += parseHex(nmea[strlen(nmea)-2]);

		// check checksum
		for (uint8_t i=2; i < (strlen(nmea)-4); i++)
		{
			sum ^= nmea[i];
		}

		if (sum)
			return false;
	}

	return true;
}


bool is_GGA_RMC_frames(char *nmea)
{

	if(!checkSum(nmea))
		return false;

	return (strstr(nmea, "$GPGGA") || strstr(nmea, "$GPRMC"));

}

bool parseData(char *nmea)
{
	char degreebuff[10];
	int32_t degree;
	long minutes;
	char * GGA_str = strstr(nmea, "$GPGGA");
	char * RMC_str = strstr(nmea, "$GPRMC");



	if (GGA_str)
	{
		//printf("GPS_raw: %s\n",strstr(nmea, "$GPGGA"));

		char *p = nmea;
	    p = strchr(p, ',')+1;
	    p = strchr(p, ',')+1;

	    if (',' != *p)
	    {
	      strncpy(degreebuff, p, 2);
	      p += 2;
	      degreebuff[2] = '\0';
	      degree = atol(degreebuff) * 10000000;
	      strncpy(degreebuff, p, 2); // minutes
	      p += 3; // skip decimal point
	      strncpy(degreebuff + 2, p, 4);
	      degreebuff[6] = '\0';
	      minutes = 50 * atol(degreebuff) / 3;
	      latitude_fixed = degree + minutes;
	      latitude = degree / 100000 + minutes * 0.000006F;
	      latitudeDegrees = (latitude-100*int(latitude/100))/60.0;
	      latitudeDegrees += int(latitude/100);
	    }

	    p = strchr(p, ',')+1;
	    p = strchr(p, ',')+1;

	    if (',' != *p)
	    {
	      strncpy(degreebuff, p, 3);
	      p += 3;
	      degreebuff[3] = '\0';
	      degree = atol(degreebuff) * 10000000;
	      strncpy(degreebuff, p, 2); // minutes
	      p += 3; // skip decimal point
	      strncpy(degreebuff + 2, p, 4);
	      degreebuff[6] = '\0';
	      minutes = 50 * atol(degreebuff) / 3;
	      longitude_fixed = degree + minutes;
	      longitude = degree / 100000 + minutes * 0.000006F;
	      longitudeDegrees = (longitude-100*int(longitude/100))/60.0;
	      longitudeDegrees += int(longitude/100);
	    }

	}

	else if (RMC_str)
	{
		//printf("GPS_raw: %s\n",strstr(nmea, "$GPRMC"));

		char *p = nmea;

	    p = strchr(p, ',')+1;
	    p = strchr(p, ',')+1;

	    if (p[0] == 'A')
	      fix = true;

	    else if (p[0] == 'V')
	      fix = false;

	    else
	      return false;

	    p = strchr(p, ',')+1;

	    if (',' != *p)
	    {
	      strncpy(degreebuff, p, 2);
	      p += 2;
	      degreebuff[2] = '\0';
	      degree = atol(degreebuff) * 10000000;
	      strncpy(degreebuff, p, 2); // minutes
	      p += 3; // skip decimal point
	      strncpy(degreebuff + 2, p, 4);
	      degreebuff[6] = '\0';
	      minutes = 50 * atol(degreebuff) / 3;
	      latitude_fixed = degree + minutes;
	      latitude = degree / 100000 + minutes * 0.000006F;
	      latitudeDegrees = (latitude-100*int(latitude/100))/60.0;
	      latitudeDegrees += int(latitude/100);
	    }

	    p = strchr(p, ',')+1;
	    p = strchr(p, ',')+1;

	    if (',' != *p)
	    {
	      strncpy(degreebuff, p, 3);
	      p += 3;
	      degreebuff[3] = '\0';
	      degree = atol(degreebuff) * 10000000;
	      strncpy(degreebuff, p, 2); // minutes
	      p += 3; // skip decimal point
	      strncpy(degreebuff + 2, p, 4);
	      degreebuff[6] = '\0';
	      minutes = 50 * atol(degreebuff) / 3;
	      longitude_fixed = degree + minutes;
	      longitude = degree / 100000 + minutes * 0.000006F;
	      longitudeDegrees = (longitude-100*int(longitude/100))/60.0;
	      longitudeDegrees += int(longitude/100);
	    }

	}

	return true;
}

bool gps(char *nmea)
{

	if(!is_GGA_RMC_frames(nmea))
	{
		//printf("Not RMC or GGA\n");
		return false;
	}

	parseData(nmea);

	if(abs(latitudeDegrees) != static_lat || abs(longitudeDegrees) != static_long)
	{
		printf("Something wrong with raw data received...\n\n");
		return false;
	}

	static int count = 0;
	printf("%d. GPS: %f -%f\n",count,latitudeDegrees,longitudeDegrees);
	count ++;
	//printf("Fix: %d\n",fix);

	return true;
}


#endif /* L5_APPLICATION_GPS_HPP_ */
