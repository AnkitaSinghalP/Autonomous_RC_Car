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
#include "motor.hpp"
#include "lpc_pwm.hpp"

static int countmia=0;
can_msg_t msg_received;
MASTER_MOTOR_CMD_t motorcmd={0};
MASTER_SYSTEM_CMD_t systemcmd;
MOTOR_HEARTBEAT_t motorheartbeat;
int rpmcount=0;
const float wheeldiam = 4.921;
const float circum = 15.451;
int prevcnt= 15.6;
float rpm=0;
GPIO *flag = NULL;

int valflag=0;


const uint32_t        MASTER_SYSTEM_CMD__MIA_MS = 1000;
const MASTER_SYSTEM_CMD_t      MASTER_SYSTEM_CMD__MIA_MSG  = {SYSTEM_STOP};


const uint32_t          MASTER_MOTOR_CMD__MIA_MS =1000 ;
const MASTER_MOTOR_CMD_t         MASTER_MOTOR_CMD__MIA_MSG = {0};


can_msg_t can_msg = { 0 };
/*can_msg_t msg_received;
MOTOR_CMD_t motorcmd;
MOTOR_HEARTBEAT_t motorheartbeat;*/



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

float getspeed(float rpm)
{
	float speedmph;
	speedmph = (rpm*circum*60)/63360;
	rpmcount=0;
	return speedmph;
}


void period_1Hz(uint32_t count)
{
	if(count==3)
	{
	rpm = rpmcount *60;
	printf("Motor RPM %f",rpm);
rpmcount=0;
	}

	CAN_bypass_filter_accept_all_msgs();
	if (CAN_rx(can1, &msg_received, 0))
	{
		dbc_msg_hdr_t can_msg_hdr;
		can_msg_hdr.dlc = msg_received.frame_fields.data_len;
		can_msg_hdr.mid = msg_received.msg_id;
		if(dbc_decode_MASTER_MOTOR_CMD(&motorcmd,msg_received.data.bytes, &can_msg_hdr))
		{

			printf("Received0 : %d \n",motorcmd.MASTER_MOTOR_CMD_speed);

		}
	}

	LE.toggle(1);


}

void period_10Hz(uint32_t count)
{

	/*valflag=1;
	if(valflag)
	flag->setHigh();*/
	static int countmia=0;
	//actspeed=getspeed(rpm);

	CAN_bypass_filter_accept_all_msgs();

	while (CAN_rx(can1, &msg_received, 0))
	{
		static bool decodeflag=0;
		static bool steerflag=0;
		dbc_msg_hdr_t can_msg_hdr;
		can_msg_hdr.dlc = msg_received.frame_fields.data_len;
		can_msg_hdr.mid = msg_received.msg_id;
		dbc_decode_MASTER_SYSTEM_CMD(&systemcmd,msg_received.data.bytes,&can_msg_hdr);
		switch(systemcmd.MASTER_SYSTEM_CMD_enum)
		{
		case SYSTEM_STOP:
			//dcmotor.set(15.0);
			//servomotor.set(14.02);
			stop();
			decodeflag=0;
			LE.on(1);
			break;
		case SYSTEM_START:
			LE.off(1);
			decodeflag=1;
			dbc_decode_MASTER_MOTOR_CMD(&motorcmd,msg_received.data.bytes, &can_msg_hdr);
			break;
		default : stop();
		//dcmotor.set(15.0);
		//servomotor.set(14.02);
		}
		if(decodeflag)
		{

			switch(motorcmd.MASTER_MOTOR_CMD_drive)
			{
			case STOP:
				LE.off(3);
				//stop();
				steerflag=0;
				stop();
				//dcmotor.set(15.0);
				//servomotor.set(14.02);
				LD.setNumber(8);
				break;
			case START:
				LE.on(3);
				LD.setNumber(0);
				steerflag=1;
				moveForward();
				//servomotor.set(14.02);
				//dcmotor.set(15.6);

				break;

			default: stop();
			//dcmotor.set(15.0);
			}

				// while(current_speed != prev_speed){
			//keep increasing the speed}
			 while(!(count%30)){
				 if (rpm == prevcnt)
				 {
					 moveForward();
				 }
			if (rpm < prevcnt)
				{

					increase_speed();
				}
				if (rpm > prevcnt)
				{

					decrease_speed();
				}
			 }
			if(steerflag)
			{

				switch(motorcmd.MASTER_MOTOR_CMD_steer)
				{


				case STEER_LEFT:
					LE.on(2);
					moveLeft();
					//servomotor.set(10.0);
					break;
				case STEER_HALF_LEFT:
					LE.on(2);
					moveHalfLeft();
					//servomotor.set(12.22);
					break;

				case STEER_RIGHT:
					LE.off(2);
					moveRight();
					//servomotor.set(20.0);
					break;
				case STEER_HALF_RIGHT:
					LE.off(2);
					moveHalfRight();
					//servomotor.set(15.82);
					break;
				case STEER_FORWARD :
					moveForward();
					//dcmotor.set(15.6);
					break;

				default:
					stop();
					//servomotor.set(14.02);
					//dcmotor.set(15.0);
				}
			}

		}
	}


	if(dbc_handle_mia_MASTER_SYSTEM_CMD(&systemcmd, 10))
	{
		LD.setNumber(countmia);
		countmia++;
		if(countmia ==99)
		{
			countmia=0;
		}
	}
	if(dbc_handle_mia_MASTER_MOTOR_CMD(&motorcmd,10))
	{
		countmia++;

		if(countmia >99)
			countmia=0;
		LD.setNumber(countmia);


		/*//dbc_msg_hdr_t msg_hdr = dbc_encode_and_send_MOTOR_HEARTBEAT(&motorheartbeat);
	can_msg.msg_id = msg_hdr.mid;

	printf("msg id %d\n", can_msg.msg_id);
	can_msg.frame_fields.data_len = msg_hdr.dlc;
	if(CAN_tx(can1, &can_msg, 0))
	{
		printf("sent 0 %d\n",can_msg.data.bytes[0]);
		printf("sent 1 %f\n",can_msg.data.bytes[1]);
		printf("sent 2 %f\n",can_msg.data.bytes[2]);

	}
		 */

		LE.toggle(2);


	}



}

void period_100Hz(uint32_t count)
{

	LE.toggle(3);

}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{
	LE.toggle(4);
}
