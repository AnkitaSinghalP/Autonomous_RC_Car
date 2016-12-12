/*
 * navigation.cpp
 *
 *  Created on: Nov 15, 2016
 *      Author: Shaurya Jain & Harsha Gorantla
 */
#include "navigation.h"

Uart2& u2 = Uart2::getInstance();
I2C2& i2c = I2C2::getInstance();

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

void Navigation::compass_init()
{
	  LPC_GPIO1->FIODIR &= ~(1<<9);
	  LPC_I2C2->I2MASK0 = 0x1F;
	  LPC_I2C2->I2CONSET = 0x40;
	  LPC_SC->PCONP |= (1<<25);

	  //Peripheral clock select
	  LPC_SC->PCLKSEL1 &= ~(3<<18);
	  LPC_SC->PCLKSEL1 |= (1<<18);

	  LPC_I2C2->I2ADR0 = COMPASS_ADDR;
}

void Navigation::compass_direction()
{
	uint8_t xMHiByte, xMLoByte, yMHiByte, yMLoByte, zMHiByte, zMLoByte;
	int16_t xMagData, yMagData, zMagData;
	float heading;

	/**
	 * CALVIN: Don't use magic numbers. This code is unmaintainable.
	 * SHAURYA:Yes, We'll change them to macros later
	 * HARSHA: Magic numbers are removed.
	 */
	i2c.writeReg(DEV_WR_ADDRESS,MODE_REG,CRA_REG_M);

	xMHiByte = i2c.readReg(DEV_RD_ADDRESS, X_MSB_REG);
	xMLoByte = i2c.readReg(DEV_RD_ADDRESS, X_LSB_REG);
	yMHiByte = i2c.readReg(DEV_RD_ADDRESS, Y_MSB_REG);
	yMLoByte = i2c.readReg(DEV_RD_ADDRESS, Y_LSB_REG);
	zMHiByte = i2c.readReg(DEV_RD_ADDRESS, Z_MSB_REG);
	zMLoByte = i2c.readReg(DEV_RD_ADDRESS, Z_LSB_REG);

	xMagData = (xMHiByte << 8) | xMLoByte;
	yMagData = (yMHiByte << 8) | yMLoByte;
	zMagData = (zMHiByte << 8) | zMLoByte;


	heading = (atan2(yMagData,xMagData)*180)/M_PI;

	if(heading < 0)
	{
		heading = MAX_ANGLE + heading;
	}

	//printf("angle = %d\n",(int)heading);



/*	if(heading >= 0 && heading <= 135)
		heading = ((heading + 45) * 2) / 3;

	else if(heading > 135 && heading <= 320)
		heading = ((heading - 40) * 9) / 7;

	else if(heading > 320 && heading <= 360)
		heading = (((heading - 320) * 3) / 4 );*/


	compass_angle = (uint16_t)heading;

}

void Navigation::compass_calibrate()
{
	/*
	 * This function is to calibrate the compass values.
	 * Store the value back to compass_angle variable after the calibration.
	 * Compass module values should match the compass on a mobile phone.
	 */

	/*
	 * We calculated these numbers after observing linear relationship between compass values and actual values.
	 * We know they seems to be magic numbers.
	 */
    if(compass_angle >= 0 && compass_angle <= 185)
        compass_angle = (compass_angle * CASE1_MUL_CAL) + CASE1_ADD_CAL;

    else if(compass_angle > 185 && compass_angle <= 340)
        compass_angle = (compass_angle * CASE2_MUL_CAL) + CASE2_ADD_CAL;

    else if(compass_angle > 340 && compass_angle <= 360)
        compass_angle = (compass_angle * CASE3_MUL_CAL) + CASE3_ADD_CAL;

}

void Navigation::gps_init()
{
	u2.init(GPS_BAUDRATE,RX_Q_SIZE,TX_Q_SIZE);
}

bool Navigation::gps_get_raw_data()
{
	if(!u2.gets(gps_raw_data,GPS_DATA_SIZE,0))
		return false;

	//printf("GPS_raw: %s\n",gps_raw_data);
	return true;
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
	      /**
	       * Calvin: too many magic numbers here.
	       * Shaurya: We agree. Will change all the required values to #define macros
	       */
	      degree = atol(degreebuff) * 10000000;
	      strncpy(degreebuff, p, 2);
	      p += 3;
	      strncpy(degreebuff + 2, p, 4);
	      degreebuff[6] = '\0';
	      minutes = 50 * atol(degreebuff) / 3;
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
	      longitude = degree / 100000 + minutes * 0.000006F;
	      longitudeDegrees = (longitude-100*int(longitude/100))/60.0;
	      longitudeDegrees += int(longitude/100);
	    }

	    /*
	     * Checking if absolute values of coordinates are for San Jose or not.
	     * Will remove this area specific checking at later stages of this project.
	     */
		if((int)latitudeDegrees != STATIC_LAT || (int)longitudeDegrees != STATIC_LONG)
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

/**
 * CALVIN: why are you returning bool?
 * SHAURYA: Yes, no need of returning anything. Changed the function to void.
 */

void Navigation::gps_calculate_bearing_angle()
{
    float x,y;

    float lat2 = next_checkpoint.latitude;//37.334709;
    float long2 = next_checkpoint.longitude;//-121.883331;

    float lat1 = coordinates.latitude;
    float long1 = coordinates.longitude;


    y = sin((long2 - long1) * (M_PI/180) )  *  cos(lat2  * (M_PI/180));


    x = (cos(lat1 * (M_PI/180)) * sin(lat2 * (M_PI/180))) - (sin(lat1 * (M_PI/180)) * cos(lat2  * (M_PI/180)) * cos((long2 - long1) * (M_PI/180)));
    gps_bearing_angle = (int)((180/M_PI) * (atan2(y,x))+360)%360;

   //printf("Bearing = %d\n",gps_bearing_angle);

}

/**
 * CALVIN: what is the reason for returning bool here?
 * SHAURYA: Yes, no need of returning anything. Changed the function to void.
 */
void Navigation::gps_calculate_distance()
{
    float a,c;

    float lat2 = next_checkpoint.latitude;//37.334709;
    float long2 = next_checkpoint.longitude;//-121.883331;

    float lat1 = coordinates.latitude;
    float long1 = coordinates.longitude;

  //  printf("GPS: %f  %f\n",lat1,long1);
    //printf("GPS: %f  %f\n",lat2,long2);


    a = pow(sin((M_PI/180)*(lat2-lat1)/2),2) + (cos((M_PI/180)*lat1))*(cos((M_PI/180)*(lat2))*pow(sin((M_PI/180)*(long2-long1)/2),2));
    c = 2 * atan2(sqrt(a),sqrt(1-a));

    gps_distance = (float)(RADIUS * c);

    if (gps_distance <= DISTANCE_OFFSET)
    	next_checkpoint_reached = true;
    else
    	next_checkpoint_reached = false;


    //printf("Distance = %f(foot)\n", gps_distance);
    /*We get Distance in Foot, since 1 Mile = 5280 Foot, we are dividing with 5280*/
    //printf("Distance = %f(miles)\n", (gps_distance/5280));

}

void Navigation::steer_command()
{
	/*
	 * Here we compare the GPS Bearing angle with the Compass true heading..
	 */

	int angle_diflection = gps_bearing_angle - compass_angle;

	if((abs(angle_diflection) < STEER_OFFSET)
			||	(abs(angle_diflection) > (MAX_ANGLE - STEER_OFFSET)))
		//steer = DIR_FORWARD;
		geo_direction.GEO_DIRECTION_enum = DIR_STRAIGHT;

/*	else if(((angle_diflection < MIN_ANGLE) && (abs(angle_diflection) < (MAX_ANGLE / 4)))
			||	((angle_diflection > MIN_ANGLE)	&& (abs(angle_diflection) > (MAX_ANGLE / 4))))
		steer = half_left;*/

	else if(((angle_diflection < MIN_ANGLE) && (abs(angle_diflection) < (MAX_ANGLE / 2)))
			||	((angle_diflection > MIN_ANGLE)	&& (abs(angle_diflection) > (MAX_ANGLE / 2))))
	{
		if(abs(angle_diflection) > MAX_ANGLE / 2)
			angle_diflection = MAX_ANGLE - abs(angle_diflection);

		if(abs(angle_diflection) < (MAX_ANGLE / 6))
			//steer = DIR_HALF_LEFT;
			geo_direction.GEO_DIRECTION_enum = DIR_HALF_LEFT;
		else
			//steer = DIR_LEFT;
			geo_direction.GEO_DIRECTION_enum = DIR_LEFT;
	}

/*
	else if(((angle_diflection < MIN_ANGLE) && (abs(angle_diflection) > (MAX_ANGLE / 4)))
			||	((angle_diflection > MIN_ANGLE) && (abs(angle_diflection) < (MAX_ANGLE / 4))))
		steer = half_right;
*/

	else if(((angle_diflection < MIN_ANGLE) && (abs(angle_diflection) >= (MAX_ANGLE / 2)))
			||	((angle_diflection > MIN_ANGLE) && (abs(angle_diflection) <= (MAX_ANGLE / 2))))
	{
		if(abs(angle_diflection) > MAX_ANGLE / 2)
			angle_diflection = MAX_ANGLE - abs(angle_diflection);

		if(abs(angle_diflection) < (MAX_ANGLE / 6))
			//steer = DIR_HALF_RIGHT;
			geo_direction.GEO_DIRECTION_enum = DIR_HALF_RIGHT;
		else
			//steer = DIR_RIGHT;
			geo_direction.GEO_DIRECTION_enum = DIR_RIGHT;
	}
}

bool Navigation::geo()
{
    compass_direction();
    compass_calibrate();

	if(!gps_get_raw_data())
		return false;

	if(!gps_raw_data_checkSum())
		return false;

	if(!is_gpgga())
		return false;

	if(!parse_gps_raw_data())
		return false;

	gps_fix = true;


	//printf("%d %d\n",last_checkpoint_received,destination_reached);

	//if next checkpoint reached
	if(last_checkpoint_received == true && destination_reached == false)
	{
		if(pop_next_checkpoint == true)
		{
			//if(queue is not empty)
			//pop coordinates from the queue and save the value to checkpoints.latitude and checkpoints.longitude

			//else
			//destination_reached = true;

			if(all_checkpoints.empty())
			{
				destination_reached = true;
				printf("Final destination reached\n");

			}
			else
			{
				next_checkpoint = all_checkpoints[0];
				all_checkpoints.erase(all_checkpoints.begin());

				printf("popping next point\n");
			}

			pop_next_checkpoint = false;
		}


		gps_calculate_distance();

		if(next_checkpoint_reached == true)
		{
			pop_next_checkpoint = true;
		}
		else
		{
			gps_calculate_bearing_angle();
			steer_command();
		}
	}

//    printf("GPS: %f  %f\n",coordinates.latitude,coordinates.longitude);
//	  printf("Bearing = %d\n",gps_bearing_angle);
//    printf("Distance = %f(foot)\n", gps_distance);
//    printf("Compass= %d\n\n",compass_angle);

	return true;
}

