/*

#include <stdio.h>

#include "motor.hpp"
#include "lpc_pwm.hpp"


float avgspeed = 15.6;


const uint16_t neutral = 15;
float halfright;
float halfleft;

float left;
float halfback;
int DClevel;

float speedfactor = 0;
float turnfactor = 0;

float actspeed;
static PWM servomotor(PWM::pwm1, 100);
static PWM dcmotor(PWM::pwm2, 100);


void initcar()
{

	servomotor.set(neutral);
	dcmotor.set(neutral);

}
void sys_reset()
{
	servomotor.set(neutral);
	dcmotor.set(neutral);
}

void stop()
{
	servomotor.set(neutral);
	dcmotor.set(neutral);
}

void start()

{
	printf("Inside start fucntion\n");
	servomotor.set(neutral);
	dcmotor.set(avgspeed);
}
void brake()
{
	servomotor.set(neutral);
	dcmotor.set(neutral);
}*/
/*void moveForward( uint16_t desiredspeed )
{
	actspeed = getactspeed();
	if(desiredspeed > actspeed)
		speedfactor = desiredspeed-actspeed;

	//if(actspeed > maxspeedfwd)
		speedfactor= 0;
}*/
/*
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

}*/


