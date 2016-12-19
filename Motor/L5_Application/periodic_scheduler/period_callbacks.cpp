/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

/**
 * @file
 * This contains the period callback functions for the periodic scheduler
 *
 * @warning
 * These callbacks should be used for hard real-time system, and the priority of these
 * tasks are above everything else in the system (above the PRIORITY_CRITICAL).
 * The period functions SHOULD NEVER block and SHOULD NEVER run over their time slot.
 * For example, the 1000Hz take slot runs periodically every 1ms, and whatever you
 * do must be completed within 1ms.  Running over the time slot will reset the system.
 */

#include <stdint.h>
#include "io.hpp"
#include "periodic_callback.h"
#include "_can_dbc/generated_can.h"
#include "stdio.h"
#include "can.h"
#include "eint.h"
#include "gpio.hpp"
#include "string.h"
#include "lpc_pwm.hpp"
#include "file_logger.h"


#define STEER_FULLLEFT 10.0
#define STEER_HALFLEFT 12.42
#define STEER_MIDDLE 14.02
#define STEER_HALFRIGHT 15.62
#define STEER_FULLRIGHT 18.04
#define THROTTLE_NEUTRAL 15.2
#define MAX_SPEED 17.5



can_msg_t msg_received;
MASTER_MOTOR_CMD_t motorcmd={STEER_STRAIGHT,STOP};
MASTER_SYSTEM_CMD_t systemcmd;
MOTOR_HEARTBEAT_t motorheartbeat;
MOTOR_SPEED_t motorspeed;


int rpmcount=0;
int speed_counter=0;
const float wheeldiam = 4.921;
const float circum = 15.451;
int prevcnt= 15.6;
float rpm=0;
int msg_systcount=0;
int  msg_motorcount=0;
int totaltx=0;
int totalrx=0;
GPIO *flag = NULL;


int valflag=0;


const uint32_t        MASTER_SYSTEM_CMD__MIA_MS = 1000;
const MASTER_SYSTEM_CMD_t      MASTER_SYSTEM_CMD__MIA_MSG  = {SYSTEM_STOP};


const uint32_t          MASTER_MOTOR_CMD__MIA_MS =1000 ;
const MASTER_MOTOR_CMD_t         MASTER_MOTOR_CMD__MIA_MSG = {STEER_STRAIGHT,STOP};




/// This is the stack size used for each of the period tasks (1Hz, 10Hz, 100Hz, and 1000Hz)
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

/**
 * This is the stack size of the dispatcher task that triggers the period tasks to run.
 * Minimum 1500 bytes are needed in order to write a debug file if the period tasks overrun.
 * This stack size is also used while calling the period_init() and period_reg_tlm(), and if you use
 * printf inside these functions, you need about 1500 bytes minimum
 */
const uint32_t PERIOD_DISPATCHER_TASK_STACK_SIZE_BYTES = (512 * 3);
void speedcounter()
{
	//flag->setLow();
	rpmcount++;
	speed_counter++;
	//printf("rpmcount %d",rpmcount);
}
/// Called once before the RTOS is started, this is a good place to initialize things once
bool period_init(void)
{
	CAN_init(can1,100,20,20,0,0);
	CAN_bypass_filter_accept_all_msgs();
	CAN_reset_bus(can1);

	eint3_enable_port2(5,eint_rising_edge,speedcounter);

	LD.setNumber(0);
	return true; // Must return true upon success
}


/// Register any telemetry variables
bool period_reg_tlm(void)
{
	// Make sure "SYS_CFG_ENABLE_TLM" is enabled at sys_config.h to use Telemetry
	return true; // Must return true upon success
}


/**
 * Below are your periodic functions.
 * The argument 'count' is the number of times each periodic task is called.
 */

bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
	can_msg_t can_msg = {0};
	can_msg.msg_id = mid;
	can_msg.frame_fields.data_len = dlc;
	memcpy(can_msg.data.bytes, bytes, dlc);
	return CAN_tx(can1, &can_msg, 0);
}

float getspeed(int rpm1)
{
	float speedmph;
	speedmph = (rpm1*circum*60)/63360;
	return speedmph;
}


void period_1Hz(uint32_t count)
{
	motorheartbeat.MOTOR_HEARTBEAT_rx_bytes = count;
	dbc_encode_and_send_MOTOR_HEARTBEAT(&motorheartbeat);

motorspeed.MOTOR_SPEED_actual = (float)speed_counter;
dbc_encode_and_send_MOTOR_SPEED(&motorspeed);

	/*if(count==3)
	{
	rpm = rpmcount *60;
	printf("Motor RPM %f",rpm);
rpmcount=0;
	}*/

	/*totalrx= msg_systcount+msg_motorcount;
	totaltx=*/
	//CAN_bypass_filter_accept_all_msgs();
	/*if (CAN_rx(can1, &msg_received, 0))
	{
		dbc_msg_hdr_t can_msg_hdr;
		can_msg_hdr.dlc = msg_received.frame_fields.data_len;
		can_msg_hdr.mid = msg_received.msg_id;
		if(dbc_decode_MASTER_MOTOR_CMD(&motorcmd,msg_received.data.bytes, &can_msg_hdr))
		{

			printf("Received0 : %d \n",motorcmd.MASTER_MOTOR_CMD_speed);

		}
	}*/

	//LE.toggle(4);
speed_counter=0;


/*
	static PWM servomotor(PWM::pwm1, 100);

	servomotor.set(15.0);
*/


}


void period_10Hz(uint32_t count)
{


	static  PWM  servomotor(PWM::pwm1, 100);
	static PWM dcmotor(PWM::pwm2, 100);

/*	static PWM dcmotor(PWM::pwm2, 100);
	static PWM servomotor(PWM::pwm1, 100);*/

	//servomotor.set(15.0);





	static float throttle_forward = THROTTLE_NEUTRAL;
	//float throttle_backward = 14.4;



	static bool decodeflag=0;
	static bool steerflag=0;


	if(throttle_forward > MAX_SPEED)
		throttle_forward = THROTTLE_NEUTRAL;
	//printf("%f\n",throttle_forward - 0.2);

		dcmotor.set(throttle_forward - 0.2);


	while (CAN_rx(can1, &msg_received, 0))
	{
		dbc_msg_hdr_t can_msg_hdr;
		can_msg_hdr.dlc = msg_received.frame_fields.data_len;
		can_msg_hdr.mid = msg_received.msg_id;
		dbc_decode_MASTER_SYSTEM_CMD(&systemcmd,msg_received.data.bytes,&can_msg_hdr);


		switch(systemcmd.MASTER_SYSTEM_CMD_enum)
		{
		case SYSTEM_STOP:
			//dcmotor.set(throttle_neutral);
			servomotor.set(STEER_MIDDLE);
			throttle_forward = THROTTLE_NEUTRAL;
			decodeflag=0;
			LD.setNumber(1);
			break;
		case SYSTEM_START:
			LE.off(1);
			decodeflag=1;

			dbc_decode_MASTER_MOTOR_CMD(&motorcmd,msg_received.data.bytes, &can_msg_hdr);
			break;
		default :
			//dcmotor.set(throttle_neutral);
			servomotor.set(STEER_MIDDLE);

		}
	}


	if(decodeflag)
	{

		switch(motorcmd.MASTER_MOTOR_CMD_drive)
		{
		case STOP:
			LD.setNumber(4);
			steerflag=0;
			throttle_forward = THROTTLE_NEUTRAL;
			//dcmotor.set(throttle_neutral);
			//servomotor.set(STEER_MIDDLE);
			break;
		case START:
			//dcmotor.set(17.0);

			steerflag=1;
			break;

		//default:
			//dcmotor.set(throttle_neutral);
		}

}


	if(steerflag)
	{

		switch(motorcmd.MASTER_MOTOR_CMD_steer)
		{

		case STEER_LEFT:
			LD.setNumber(3);
			servomotor.set(STEER_FULLLEFT);

			break;
		case STEER_HALF_LEFT:
			LD.setNumber(3);
			servomotor.set(STEER_HALFLEFT);
			break;

		case STEER_RIGHT:
			LD.setNumber(1);
			servomotor.set(STEER_FULLRIGHT);

			break;

		case STEER_HALF_RIGHT:
			LD.setNumber(1);
			servomotor.set(STEER_HALFRIGHT);

			break;

		case STEER_STRAIGHT :
			LD.setNumber(2);
			servomotor.set(STEER_MIDDLE);

			break;

		default:
			LD.setNumber(4);
			servomotor.set(STEER_MIDDLE);
		}

		//dcmotor.set(20.0);
/*		if(throttle_forward < 16.05)
		{

			throttle_forward = throttle_forward + 0.05;

		}*/



		if(rpmcount < 1)
		{
			throttle_forward = throttle_forward + 0.03;
		}
		else if(rpmcount > 1)
		{
			throttle_forward = throttle_forward - 0.03;
		}


	}

	//printf("rpmcount %d",rpmcount);
	//motorspeed.MOTOR_SPEED_actual=getspeed(rpmcount);

	rpmcount=0;

}

void period_100Hz(uint32_t count)
{

	//LE.toggle(3);

}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{
	LE.toggle(4);
}
