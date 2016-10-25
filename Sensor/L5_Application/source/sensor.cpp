/*
 * sensor.cpp
 *
 *  Created on: Oct 18, 2016
 *      Author: BHUSHAN
 */
#include "sensor.hpp"

#define TIMER_TICK 1
static GPIO *middele_sensor_pin= NULL;
static GPIO *middele_echo_pin= NULL;
static GPIO *right_sensor_pin= NULL;
static GPIO *right_echo_pin= NULL;
static GPIO *left_sensor_pin= NULL;
static GPIO *left_echo_pin= NULL;
static GPIO *rear_sensor_pin= NULL;
static GPIO *rear_echo_pin= NULL;
static GPIO *rear_left_sensor_pin= NULL;
static GPIO *rear_left_echo_pin= NULL;
static GPIO *IR_sensor_pin = NULL;

static int start_time_middle=0, actual_time_middle;
static int start_time_right=0, actual_time_right =0;
static int start_time_left=0, actual_time_left;
static int start_time_rear=0, actual_time_rear =0;


void sensor_init(){
	lpc_timer_enable(lpc_timer0, TIMER_TICK);

	if((middele_sensor_pin==NULL) & (middele_echo_pin == NULL))
	{
		middele_sensor_pin = new GPIO(P1_22);
		middele_echo_pin = new GPIO(P2_1);
	}
	if((right_sensor_pin==NULL) & (right_echo_pin == NULL))
	{
		right_sensor_pin = new GPIO(P1_23);
		right_echo_pin = new GPIO(P2_3);

	}
	if((left_sensor_pin==NULL) & (middele_echo_pin == NULL))
	{
		left_sensor_pin = new GPIO(P1_28);
		left_echo_pin = new GPIO(P2_5);
	}
	if((rear_sensor_pin==NULL) & (rear_echo_pin == NULL))
	{
		rear_sensor_pin = new GPIO(P1_29);
		rear_echo_pin = new GPIO(P2_7);

	}
	if((IR_sensor_pin==NULL) & (rear_left_echo_pin == NULL))
	{
		rear_left_sensor_pin = new GPIO(P2_6);
		rear_left_echo_pin = new GPIO(P2_7);

	}
	middele_sensor_pin-> setAsOutput();
	middele_echo_pin-> setAsInput();
	right_sensor_pin-> setAsOutput();
	right_echo_pin->setAsInput();
	left_sensor_pin-> setAsOutput();
	left_echo_pin-> setAsInput();
	rear_sensor_pin-> setAsOutput();
	rear_echo_pin->setAsInput();
	//rear_left_sensor_pin-> setAsOutput();
	//rear_left_echo_pin-> setAsInput();

}

void isr_middle1()
{
	//puts("inside isr2 \n");
	actual_time_middle = lpc_timer_get_value(lpc_timer0) - start_time_middle;
	//sensor_pin->setLow();
	portYIELD_FROM_ISR(NULL);
}

void isr_right1()
{
	//puts("inside isr3 \n");
	actual_time_right = lpc_timer_get_value(lpc_timer0) - start_time_right;
	portYIELD_FROM_ISR(NULL);
}
void isr_left1()
{
	//puts("inside isr2 \n");
	actual_time_left = lpc_timer_get_value(lpc_timer0) - start_time_left;
	//sensor_pin->setLow();
	portYIELD_FROM_ISR(NULL);
}

void isr_rear1()
{
	//puts("inside isr3 \n");
	actual_time_rear = lpc_timer_get_value(lpc_timer0) - start_time_rear;
	portYIELD_FROM_ISR(NULL);
}

void isr_middle()
{
	//puts("inside isr1");
	start_time_middle = lpc_timer_get_value(lpc_timer0);
	//eint3_enable_port2(1,eint_falling_edge,isr2);
	eint3_enable_port2(1,eint_falling_edge,isr_middle1);
	//puts("after eint2");
	portYIELD_FROM_ISR(NULL);
}
void isr_right()
{
	//puts("inside isr1");
	start_time_right = lpc_timer_get_value(lpc_timer0);
	//eint3_enable_port2(1,eint_falling_edge,isr2);
	eint3_enable_port2(3,eint_falling_edge,isr_right1);
	//puts("after eint2");
	portYIELD_FROM_ISR(NULL);
}
void isr_left()
{
	//puts("inside isr1");
	start_time_left = lpc_timer_get_value(lpc_timer0);
	//eint3_enable_port2(1,eint_falling_edge,isr2);
	eint3_enable_port2(5,eint_falling_edge,isr_left1);
	//puts("after eint2");
	portYIELD_FROM_ISR(NULL);
}
void isr_rear()
{
	//puts("inside isr1");
	start_time_rear = lpc_timer_get_value(lpc_timer0);
	//eint3_enable_port2(1,eint_falling_edge,isr2);
	eint3_enable_port2(7,eint_falling_edge,isr_rear1);
	//puts("after eint2");
	portYIELD_FROM_ISR(NULL);
}


int sensor_sonar_middle_trigger(){
	int distance_middle= 0.0;
	middele_sensor_pin-> setLow();
	middele_sensor_pin->setHigh();
	delay_us(20);
	middele_sensor_pin->setLow();
	eint3_enable_port2(1,eint_rising_edge,isr_middle);
	distance_middle = actual_time_middle / 147;
	printf("distance %d \n", distance_middle);

	return distance_middle;
}

int sensor_sonar_right_trigger(){
	int distance_right= 0.0;
	right_sensor_pin-> setLow();
	right_sensor_pin->setHigh();
	delay_us(20);
	right_sensor_pin->setLow();
	eint3_enable_port2(3,eint_rising_edge,isr_right);
	distance_right = actual_time_right / 147;
	printf("distance %d \n", distance_right);

	return distance_right;
}

int sensor_sonar_left_trigger(){
	int distance_left= 0.0;
	right_sensor_pin-> setLow();
	right_sensor_pin->setHigh();
	delay_us(20);
	right_sensor_pin->setLow();
	eint3_enable_port2(5,eint_rising_edge,isr_left);
	distance_left = actual_time_left / 147;
	printf("distance %d \n", distance_left);

	return distance_left;
}

int sensor_sonar_rear_trigger(){
	int distance_rear= 0.0;
	right_sensor_pin-> setLow();
	right_sensor_pin->setHigh();
	delay_us(20);
	right_sensor_pin->setLow();
	eint3_enable_port2(7,eint_rising_edge,isr_rear);
	distance_rear = actual_time_rear / 147;
	printf("distance %d \n", distance_rear);

	return distance_rear;
}

/*
float sensor_sonar_rear_left_trigger(){
	int distance_rear_left= 0.0;
	right_sensor_pin-> setLow();
	right_sensor_pin->setHigh();
	delay_us(20);
	right_sensor_pin->setLow();
	eint3_enable_port2(7,eint_rising_edge,isr1);
	distance_rear_left = actual_time_middle / 147;
	printf("distance %f \n", distance_rear_left);

	return distance_rear_left;
}
*/

