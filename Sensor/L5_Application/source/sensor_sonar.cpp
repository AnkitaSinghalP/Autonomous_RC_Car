/*
 * sensor.cpp
 *
 *  Created on: Oct 18, 2016
 *      Author: BHUSHAN
 */
#include <sensor_sonar.hpp>

#include "eint.h"
#include "tasks.hpp"
#include "shared_handles.h"
#include "scheduler_task.hpp"

#include "lpc_timers.h"
#include "utilities.h"

#define TIMER_TICK 1

sensor_readings obstacle;

static int start_time_middle=0, actual_time_middle;
static int start_time_right=0, actual_time_right =0;
static int start_time_left=0, actual_time_left;
static int start_time_rear=0, actual_time_rear =0;


void isr_middle1()
{
	//puts("inside isr2 \n");
	actual_time_middle = lpc_timer_get_value(lpc_timer0) - start_time_middle;
	obstacle.middle_distance = actual_time_middle / 147;
	portYIELD_FROM_ISR(0);
}

void isr_right1()
{
	//puts("inside isr3 \n");
	actual_time_right = lpc_timer_get_value(lpc_timer0) - start_time_right;
	obstacle.right_distance = actual_time_right / 147;
	portYIELD_FROM_ISR(0);
}
void isr_left1()
{
	//puts("inside isr2 \n");
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
	LPC_GPIO1->FIOPIN &= ~(1 << 22);
	LPC_GPIO1->FIOPIN |= (1 << 22);
	delay_us(SENSOR_DELAY);
	LPC_GPIO1->FIOPIN &= ~(1 << 22);
}

void sensor_sonar_right_trigger()
{
	LPC_GPIO1->FIOPIN &= ~(1 << 23);
	LPC_GPIO1->FIOPIN |= (1 << 23);
	delay_us(SENSOR_DELAY);
	LPC_GPIO1->FIOPIN &= ~(1 << 23);
}

void sensor_sonar_left_trigger()
{
	LPC_GPIO1->FIOPIN &= ~(1 << 28);
	LPC_GPIO1->FIOPIN |= (1 << 28);
	delay_us(SENSOR_DELAY);
	LPC_GPIO1->FIOPIN &= ~(1 << 28);
}

void sensor_sonar_rear_trigger()
{
	LPC_GPIO1->FIOPIN &= ~(1 << 29);
	LPC_GPIO1->FIOPIN |= (1 << 29);
	delay_us(SENSOR_DELAY);
	LPC_GPIO1->FIOPIN &= ~(1 << 29);
}

void interrupt_enable()
{
	eint3_enable_port2(1,eint_rising_edge,isr_middle);
	eint3_enable_port2(3,eint_rising_edge,isr_right);
	eint3_enable_port2(5,eint_rising_edge,isr_left);
	eint3_enable_port2(7,eint_rising_edge,isr_rear);

	eint3_enable_port2(1,eint_falling_edge,isr_middle1);
	eint3_enable_port2(3,eint_falling_edge,isr_right1);
	eint3_enable_port2(5,eint_falling_edge,isr_left1);
	eint3_enable_port2(7,eint_falling_edge,isr_rear1);

}
void sensor_receiver_pins()
{
	LPC_GPIO2->FIODIR |= (1 << 1);
	LPC_GPIO2->FIODIR |= (1 << 3);
	LPC_GPIO2->FIODIR |= (1 << 5);
	LPC_GPIO2->FIODIR |= (1 << 7);
}
bool sensor_init(){

	sensor_receiver_pins();
	delay_ms(1);
	interrupt_enable();
	lpc_timer_enable(lpc_timer0, TIMER_TICK);
	return true;

}

bool sensor_measure()
{
	sensor_sonar_middle_trigger();
	sensor_sonar_rear_trigger();
	delay_ms(10);
	sensor_sonar_right_trigger();
	sensor_sonar_left_trigger();


	return true;

}

