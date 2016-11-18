/*
 * sensor.cpp
 *
 *  Created on: Oct 18, 2016
 *      Author: BHUSHAN
 *
 *      PIN CONNECTIONS:
 *      Left: PW:2.5  RX:1.28
 *      Middle: PW:2.3 RX:1.23
 *      Right:  PW:2.1 RX: 1.22
 *      Rear:  PW:2.7 RX:1.29
 */
#include <sensor_sonar.hpp>

#include "eint.h"
#include "tasks.hpp"
#include "shared_handles.h"
#include "scheduler_task.hpp"
#include "can.h"
#include "lpc_timers.h"
#include <stdint.h>
#include "utilities.h"
#include "_can_dbc/generated_can.h"
#include "io.hpp"
#include "stdio.h"
#include "string.h"

#define TIMER_TICK 1

sensor_readings obstacle;

static int start_time_middle=0, actual_time_middle = 0;
static int start_time_right=0, actual_time_right =0;
static int start_time_left=0, actual_time_left = 0;
static int start_time_rear=0, actual_time_rear =0;


SENSOR_HEARTBEAT_t sensor_heartbeat_message = {0};
//can_msg_t can_msg_sensor = { 0 };
can_msg_t can_msg_received;
can_msg_t can_msg = { 0 };

const uint32_t SYSTEM_CMD__MIA_MS = 1000;
const SYSTEM_CMD_t SYSTEM_CMD__MIA_MSG = {SYSTEM_STOP};

SENSOR_ULTRASONIC_m0_t ultrasonic_sensor_data = {0};

SENSOR_ULTRASONIC_t ultrasonic_sensor_receiver = {0};

SYSTEM_CMD_t master_command;

dbc_mia_info_t mia_handling = {0};

SENSOR_BATT_t battery_status = {0};

dbc_msg_hdr_t can_msg_hdr;

int front_data[3] = {0};
int right_data[3] = {0};
int left_data[3] = {0};
int rear_data[3] = {0};

static int counter = 0;

void decoded_can_sensor_message(uint8_t middle,uint8_t left, uint8_t right)
{
	// obstacle.left_distance = 10;

	/*  printf("left dist %d\n",obstacle.left_distance);
    printf("right dist %d\n",obstacle.right_distance);
    printf("rear dist %d\n",obstacle.rear_distance);
    printf("middle dist %d\n",obstacle.middle_distance);*/
	ultrasonic_sensor_data.SENSOR_ULTRASONIC_middle = middle;
	ultrasonic_sensor_data.SENSOR_ULTRASONIC_left = left;
	ultrasonic_sensor_data.SENSOR_ULTRASONIC_right = right;
	/*  ultrasonic_sensor_data.SENSOR_ULTRASONIC_rear_right = obstacle.rear_distance;
    ultrasonic_sensor_data.SENSOR_ULTRASONIC_rear_left = 55;*/

	dbc_encode_and_send_SENSOR_ULTRASONIC_m0(&ultrasonic_sensor_data);
	LD.setNumber(obstacle.middle_distance);

}


bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{

	can_msg.msg_id = mid;
	can_msg.frame_fields.data_len = dlc;
	memcpy(can_msg.data.bytes, bytes, dlc);
	return CAN_tx(can1, &can_msg, 0);
}

void ultrasonic_sensor_heartbeat_message()
{
	sensor_heartbeat_message.SENSOR_HEARTBEAT_tx_bytes = 12;
	sensor_heartbeat_message.SENSOR_HEARTBEAT_rx_bytes = 54;
	dbc_encode_and_send_SENSOR_HEARTBEAT(&sensor_heartbeat_message);
}


void isr_middle1()
{
	// puts("inside isr_middle1 \n");
	actual_time_middle = lpc_timer_get_value(lpc_timer0) - start_time_middle;
	obstacle.middle_distance = actual_time_middle / 147;
	portYIELD_FROM_ISR(0);
}

void isr_right1()
{
	// puts("inside isr_right1 \n");
	actual_time_right = lpc_timer_get_value(lpc_timer0) - start_time_right;
	obstacle.right_distance = actual_time_right / 147;

	portYIELD_FROM_ISR(0);
}
void isr_left1()
{
	// puts("inside isr_left1 \n");
	actual_time_left = lpc_timer_get_value(lpc_timer0) - start_time_left;
	obstacle.left_distance = actual_time_left / 147;

	portYIELD_FROM_ISR(0);
}

void isr_rear1()
{
	//puts("inside isr3 \n");
	actual_time_rear = lpc_timer_get_value(lpc_timer0) - start_time_rear;
	obstacle.rear_distance = actual_time_rear / 147;
	portYIELD_FROM_ISR(0);
}

void isr_middle()
{
	//puts("inside isr1");
	start_time_middle = lpc_timer_get_value(lpc_timer0);


}
void isr_right()
{
	//puts("inside isr1");
	start_time_right = lpc_timer_get_value(lpc_timer0);

}
void isr_left()
{
	//puts("inside isr1");
	start_time_left = lpc_timer_get_value(lpc_timer0);

}
void isr_rear()
{
	//puts("inside isr1");
	start_time_rear = lpc_timer_get_value(lpc_timer0);

}


void sensor_sonar_middle_trigger()
{

	LPC_GPIO1->FIOPIN |= (1 << 23 || 1 << 29);
	//LPC_GPIO1->FIOPIN |= (1 << 29);
	delay_us(SENSOR_DELAY);
	LPC_GPIO1->FIOPIN &= ~(1 << 23 || 1 << 29);
	//LPC_GPIO1->FIOPIN &= ~(1 << 29);

}

void sensor_sonar_right_trigger()
{
	//LPC_GPIO1->FIOPIN |= (1 << 28);
	LPC_GPIO1->FIOPIN |= (1 << 22 || 1 << 28);
	delay_us(SENSOR_DELAY);
	LPC_GPIO1->FIOPIN &= ~(1 << 22 || 1 << 28);
//	LPC_GPIO1->FIOPIN &= ~(1 << 28);


}

void sensor_sonar_left_trigger()
{


	//delay_us(SENSOR_DELAY);

}

void sensor_sonar_rear_trigger()
{
	/*LPC_GPIO1->FIODIR |= (1 << 29);
	LPC_GPIO1->FIOPIN &= ~(1 << 29);
	LPC_GPIO1->FIOPIN |= (1 << 29);
	delay_us(SENSOR_DELAY);
	LPC_GPIO1->FIOPIN &= ~(1 << 29);*/
}

void interrupt_enable()
{
	eint3_enable_port2(1,eint_rising_edge,isr_right);
	eint3_enable_port2(3,eint_rising_edge,isr_middle);
	eint3_enable_port2(5,eint_rising_edge,isr_left);
	eint3_enable_port2(7,eint_rising_edge,isr_rear);

}
void interrupt_falling_edge_enable()
{
	eint3_enable_port2(1,eint_falling_edge,isr_right1);
	eint3_enable_port2(3,eint_falling_edge,isr_middle1);
	eint3_enable_port2(5,eint_falling_edge,isr_left1);
	eint3_enable_port2(7,eint_falling_edge,isr_rear1);
}
void sensor_receiver_pins()
{
	/*LPC_GPIO2->FIODIR &= ~(1 << 1);
    LPC_GPIO2->FIODIR &= ~(1 << 3);
    LPC_GPIO2->FIODIR &= ~(1 << 5);
    LPC_GPIO2->FIODIR &= ~(1 << 7);*/
}

void can_init_sensor()
{
	CAN_init(can1, 100, 20, 20,0,0);
	CAN_bypass_filter_accept_all_msgs();
	CAN_reset_bus(can1);

	//return true;

}

void sensor_init(){

	sensor_receiver_pins();
	interrupt_enable();
	lpc_timer_enable(lpc_timer0, TIMER_TICK);
	interrupt_falling_edge_enable();

}
void compute()
{
	uint8_t left,right,middle;
	// printf(" %d     %d      %d\n",obstacle.left_distance,obstacle.middle_distance,obstacle.right_distance);
	LD.setNumber(0);
	if(obstacle.right_distance<=30){
		LE.toggle(1);
		right=1;

	}
	else{
		right=0;
	}
	if(obstacle.middle_distance<=50){
		middle=1;
		LE.toggle(2);
	}
	else{
		middle=0;
	}
	if (obstacle.left_distance<=30) {
		left=1;
		LE.toggle(3);
	}

	else{
		left=0;
	}
	/*
switch((obstacle.left_distance>0) ||(obstacle.middle_distance >0)||(obstacle.right_distance > 0)){
case ((obstacle.left_distance > 15) && (obstacle.left_distance<=30)):
//obstacle on left turn right
case ((obstacle.middle_distance > 15) && (obstacle.middle_distance<=45)):
//obstacle in front
case ((obstacle.right_distance > 15) && (obstacle.right_distance<=30)):
//obstacle on right
case(obstacle.left_distance <= 15||obstacle.right_distance <= 15||obstacle.middle_distance < 15):
//critical stop
default:
//no obstacle

}*/

	decoded_can_sensor_message(middle,left,right);
}

void sensor_measure()
{
	sensor_sonar_middle_trigger();
	//sensor_sonar_rear_trigger();
	delay_ms(10);
	sensor_sonar_right_trigger();
	//delay_ms(10);
	//sensor_sonar_left_trigger();
	compute();

}
void can_communication_sensor()
{
	static int counter_send = 0;
	counter_send++;
	// decoded_can_sensor_message();
	if(counter_send % 100==0)
		ultrasonic_sensor_heartbeat_message();

}

uint8_t received_sensor_can_msg()
{

	while(CAN_rx(can1, &can_msg_received, 0))
	{
		can_msg_hdr.dlc = can_msg_received.frame_fields.data_len;
		can_msg_hdr.mid = can_msg_received.msg_id;
		if(dbc_decode_SYSTEM_CMD(&master_command, can_msg_received.data.bytes, &can_msg_hdr)){
		}
	}

	if(dbc_handle_mia_SYSTEM_CMD(&master_command, 10))
	{


		if(counter == 99 )

		{
			counter =0;
		}
		counter++;
	}
	if(master_command.SYSTEM_CMD_enum == SYSTEM_START){
		return 1;
	}
	else{
		return 0;
	}

}

