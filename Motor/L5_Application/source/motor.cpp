
#include <stdio.h>
#include "lpc_pwm.hpp"
const float avgspeed =15.6;
const float servoneutral = 14.02;
const float maxspeedfwd = 20.0;
const float maxspeedbck=10.0;

const float neutral = 15;
const float halfright = 15.82;
const float halfleft = 12.22;
const float fullleft = 10.0;
const float fullright = 20.0;
float left;
float right;


float back;
int DClevel;

float speedfactor = 0;
float turnfactor = 0;
float speedoffset = 15.6;
float maxspeedoffset=17.6;

PWM servomotor(PWM::pwm1, 100);
PWM dcmotor(PWM::pwm2, 100);


void initcar()
{

	//servomotor.set(neutral);
	dcmotor.set(neutral+2);

}
void increase_speed()
{
	//speedoffset = avgspeed +0.1;
	while(speedoffset<=17.6){
	dcmotor.set(speedoffset);
	speedoffset = avgspeed +0.1;
}
}
void decrease_speed()
{

	while(maxspeedoffset>=15.6)
	{
		dcmotor.set(maxspeedoffset);
		maxspeedoffset = avgspeed -0.1;
	}

}

void moveForward( )
{
	/*float front=15.0;
	float speedoffset=0.0;
	actspeed = getactspeed();*/

	dcmotor.set(avgspeed);
	servomotor.set(servoneutral);
	/*if(desiredspeed > actspeed)
	{
		speedfactor = desiredspeed-actspeed;

		while((front+speedoffset)<desiredspeed)
		{
			speedoffset = speedfactor *0.2;

			if(front<=desiredspeed)
				dcmotor.set(front+speedoffset);
			speedfactor++;
		}
	}
	else if(actspeed > maxspeedfwd)
		{
			speedfactor= 0;
			dcmotor.set(avgspeed);
		}*/

}


	void moveBack( float desiredspeed)
	{
		desiredspeed = avgspeed;
	}
	void moveRight( )
	{
		/*dcmotor.set(avgspeed);

		turnfactor = halfright;*/
		servomotor.set(fullright);
	}
	void moveHalfRight( )
		{
			/*dcmotor.set(avgspeed);

			turnfactor = halfright;*/
			servomotor.set(halfright);
		}
	void moveLeft()
	{
		//desiredturn = avgspeed;
		//turnfactor = halfleft;
		servomotor.set(fullleft);
	}
	void moveHalfLeft()
		{
			//desiredturn = avgspeed;
			//turnfactor = halfleft;
			servomotor.set(halfleft);
		}
	void stop()
	{
		turnfactor = 0;
		dcmotor.set(neutral);
		servomotor.set(servoneutral);
	}

