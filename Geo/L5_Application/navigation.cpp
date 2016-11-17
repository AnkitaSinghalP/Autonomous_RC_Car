/*
 * navigation.cpp
 *
 *  Created on: Nov 15, 2016
 *      Author: Shaurya Jain & Harsha Gorantla
 */
#include "navigation.h"

Uart2& u2 = Uart2::getInstance();

uint8_t parse_hex(char c)
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

void Navigation::gps_init()
{
	u2.init(GPS_BAUDRATE,RX_Q_SIZE,TX_Q_SIZE);
}

void Navigation::gps_get_raw_data()
{
	u2.gets(gps_raw_data,GPS_DATA_SIZE,0);

	//printf("GPS_raw: %s\n",gps_raw_data);

}

bool Navigation::is_gpgga()
{
	return (strstr(gps_raw_data, "$GPGGA"));
}

bool Navigation::gps_raw_data_checkSum()
{
	if (gps_raw_data[strlen(gps_raw_data)-4] == '*')
	{
		uint16_t sum = parse_hex(gps_raw_data[strlen(gps_raw_data)-3]) * 16;
		sum += parse_hex(gps_raw_data[strlen(gps_raw_data)-2]);

		for (uint8_t i=2; i < (strlen(gps_raw_data)-4); i++)
		{
			sum ^= gps_raw_data[i];
		}

		if (sum)
			return false;
	}

	return true;
}

bool Navigation::parse_gps_raw_data()
{
	char degreebuff[10];
	int32_t degree;
	//int32_t latitude_fixed, longitude_fixed;
	long minutes;
	float latitude, longitude;
	float latitudeDegrees, longitudeDegrees;

	if (strstr(gps_raw_data, "$GPGGA"))
	{
		char *p = gps_raw_data;

	    p = strchr(p, ',')+1;
	    p = strchr(p, ',')+1;

	    if (',' != *p)
	    {
	      strncpy(degreebuff, p, 2);
	      p += 2;
	      degreebuff[2] = '\0';
	      degree = atol(degreebuff) * 10000000;
	      strncpy(degreebuff, p, 2);
	      p += 3;
	      strncpy(degreebuff + 2, p, 4);
	      degreebuff[6] = '\0';
	      minutes = 50 * atol(degreebuff) / 3;
	      //latitude_fixed = degree + minutes;
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
	      //longitude_fixed = degree + minutes;
	      longitude = degree / 100000 + minutes * 0.000006F;
	      longitudeDegrees = (longitude-100*int(longitude/100))/60.0;
	      longitudeDegrees += int(longitude/100);
	    }

	    /*
	     * Checking if absolute values of coordinates are for San Jose or not.
	     * Will remove this area specific checking at later stages of this project.
	     */
		if(abs(latitudeDegrees) != STATIC_LAT || abs(longitudeDegrees) != STATIC_LONG)
			return false;

		/*
		 * Negating longitude for San Jose
		 * Will remove this area specific assignment at later stages of this project.
		 * Will consider the E/W values of the gps_raw_data
		 */
		longitudeDegrees = 0 - longitudeDegrees;


	    coordinates.latitude 	= latitudeDegrees;
	    coordinates.longitude 	= longitudeDegrees;

	}

	return true;

}

bool Navigation::gps_calculate_bearing_angle()
{
    float x,y;

    float lat1 = 37.335808;
    float long1 = -121.882744;

    float lat2 = coordinates.latitude;//37.336227;
    float long2 = coordinates.longitude;//-121.881882;


    y = sin((long2 - long1) * (M_PI/180) )  *  cos(lat2  * (M_PI/180));


    x = (cos(lat1 * (M_PI/180)) * sin(lat2 * (M_PI/180))) - (sin(lat1 * (M_PI/180)) * cos(lat2  * (M_PI/180)) * cos((long2 - long1) * (M_PI/180)));
    gps_bearing_angle = (int)((180/M_PI) * (atan2(y,x))+360)%360;

    printf("Bearing = %d\n",gps_bearing_angle);

    return true;
}

bool Navigation::gps_calculate_distance()
{
    float a,c;

    float lat1 = 37.336227;
    float long1 = -121.881882;

    float lat2 = coordinates.latitude;
    float long2 = coordinates.longitude;


    a = pow(sin((M_PI/180)*(lat2-lat1)/2),2) + (cos((M_PI/180)*lat1))*(cos((M_PI/180)*(lat2))*pow(sin((M_PI/180)*(long2-long1)/2),2));
    c = 2 * atan2(sqrt(a),sqrt(1-a));

    gps_distance = (float)(RADIUS * c);

    //printf("Distance = %f(foot)\n", gps_distance);
    /*We get Distance in Foot, since 1 Mile = 5280 Foot, we are dividing with 5280*/
    printf("Distance = %f(miles)\n", (gps_distance/5280));

    return true;
}

bool Navigation::geo()
{
	gps_get_raw_data();

	if(!gps_raw_data_checkSum())
		return false;

	if(!is_gpgga())
		return false;

	if(!parse_gps_raw_data())
		return false;

    printf("GPS: %f  %f\n",coordinates.latitude,coordinates.longitude);

    if(!gps_calculate_bearing_angle())
    	return false;

    if(!gps_calculate_distance())
            return false;

	return true;
}


