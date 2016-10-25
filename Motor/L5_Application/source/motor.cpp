#include <stdio.h>

#include "motor.hpp"
#include "lpc_pwm.hpp"

 uint16_t pickupspeeed;
 uint16_t avgspeed;
 uint16_t maxspeedfwd;
 uint16_t maxspeedbck;

 const uint16_t neutral = 15;
 float halfright;
 float halfleft;

 float left;
 float halfback;
 int DClevel;

float speedfactor = 0;
float turnfactor = 0;
float actspeed;



void initcar()
{
	extern static PWM servomotor(PWM::pwm1, 100);
	extern static PWM dcmotor(PWM::pwm2, 100);
	servomotor.set(neutral);
	dcmotor.set(neutral);

}
void moveForward( uint16_t desiredspeed )
{
	actspeed = getactspeed();
	if(desiredspeed > actspeed)
		speedfactor = desiredspeed-actspeed;

	if(actspeed > maxspeedfwd)
		speedfactor= 0;
}
void moveBack( uint16_t desiredspeed)
{
desiredspeed = avgspeed;
halfback= 12.5;

}
void moveRight( uint16_t desiredspeed)
{
desiredspeed = avgspeed;
turnfactor = halfright;
}
void moveLeft( uint16_t desiredspeed)
{
desiredspeed = avgspeed;
turnfactor = halfleft;

}
