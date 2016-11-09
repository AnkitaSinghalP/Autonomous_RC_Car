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
 *
 *
 */

#include <stdint.h>
#include "io.hpp"
#include "periodic_callback.h"
#include "can.h"
#include <stdio.h>
#include "_can_dbc/generated_can.h"
#include "string.h"
#include "LED_Display.hpp"
#include "lpc_isr.h"
#include "eint.h"
#include "uart2.hpp"
#include "uart_dev.hpp"

//#define UART2_POWER_ENABLE (0x01000000) /* UART 2 Power Enable */
//#define UART2_WORD_LENGTH_8 (0x00000003) /* 8 bit World Length */
//#define UART2_DLAB_ENABLE (0x00000080) /* DLAB Enable */
//#define PCLK (48000000)
//#define BAUDRATE (115200)

volatile int receive_data = 0;

bool flag1;
can_std_id_t id;
bool flag_tx;
can_msg_t abc;

SYSTEM_CMD_t system_cmd = {SYSTEM_STOP};
GEO_LOCATION_t geo_location = { 0 };
can_msg_t can_msg = { 0 };

BLE_HEARTBEAT_t ble_heartbeat_t = { 0 };
BLE_CHCK_PT_t ble_chk_pt = { 0 };
BLE_COMM_CMD_t ble_cmd = {COMM_STOP};
BLE_MAP_DATA_t ble_map_data = { 0 };

SemaphoreHandle_t receive_sem;

//static int count1 = 0;
//const uint16_t ravail=(2<<1);
static char rx_data;

const uint32_t SYSTEM_CMD__MIA_MS = 1000;
const SYSTEM_CMD_t SYSTEM_CMD__MIA_MSG = { SYSTEM_STOP };

/*const uint32_t GEO_LOCATION__MIA_MS = 1000;
const GEO_LOCATION_t GEO_LOCATION__MIA_MSG = { 0 };*/

/// This is the stack size used for each of the period tasks (1Hz, 10Hz, 100Hz, and 1000Hz)
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

/**
 * This is the stack size of the dispatcher task that triggers the period tasks to run.
 * Minimum 1500 bytes are needed in order to write a debug file if the period tasks overrun.
 * This stack size is also used while calling the period_init() and period_reg_tlm(), and if you use
 * printf inside these functions, you need about 1500 bytes minimum
 */
const uint32_t PERIOD_DISPATCHER_TASK_STACK_SIZE_BYTES = (512 * 3);

/// Called once before the RTOS is started, this is a good place to initialize things once
Uart2& u2 = Uart2 :: getInstance();
//void UART2_int_handler(void)
//	{
//	receive_data=1;
//		NVIC_DisableIRQ(UART2_IRQn);
//
//	}

//void uart_init_interrupt(void)
//	{
//				LPC_PINCON->PINSEL4 &= ~(0xF << 16); // Clear values
//			    LPC_PINCON->PINSEL4 |=  (0xA << 16); // Set values for UART2 Rx/Tx
//
//			    LPC_SC->PCONP |= UART2_POWER_ENABLE;
//
//			    LPC_SC->PCLKSEL1 &= ~(3 << 16);
//			    LPC_SC->PCLKSEL1 |=  (1 << 16);
//			    LPC_UART2->LCR = (1 << 7);  // Enable DLAB
//			    LPC_UART2->DLM = 0;
//			   // LPC_UART2->LCR = UART2_WORD_LENGTH_8 | UART2_DLAB_ENABLE;
//			    LPC_UART2->DLL = (PCLK/16)/BAUDRATE;
//			    LPC_UART2->LCR = UART2_WORD_LENGTH_8; // DLAB =
//			    LPC_UART2->LCR &=~(1<<7);
//			    LPC_UART2->IER |=(1<<0);
//			    isr_register(UART2_IRQn,UART2_int_handler);
//	}

bool period_init(void)
{

	//uart_init_interrupt();
	u2.init(115200,100,100);
	//vSemaphoreCreateBinary(receive_sem);
	CAN_init(can1, 100, 20, 20, 0, 0);
	CAN_bypass_filter_accept_all_msgs();
	CAN_reset_bus(can1);
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



//unsigned long uart2_rx_interrupt(char *receive)
//	{
//	      while(!(LPC_UART2->LSR &(1<<0)));
//
//	      *receive = LPC_UART2->RBR;
//
//	      return 1;
//	}
//
//	unsigned long uart2_tx_interrupt(char rx_data1)
//	{
//
//		LPC_UART2->THR = rx_data1;
//		while(!(LPC_UART2->LSR &(1<<5)));
//		return 1;
//	}
//
//
//void receive_Handler(void){
//
//	xSemaphoreGiveFromISR(receive_sem,NULL);
//
//}

bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
	can_msg.msg_id = mid;
	can_msg.frame_fields.data_len = dlc;
	memcpy(can_msg.data.bytes, bytes, dlc);
	return CAN_tx(can1, &can_msg, 0);
}

void period_1Hz(uint32_t count)
{

	if(CAN_is_bus_off(can1)){
			CAN_reset_bus(can1);
		}

	ble_heartbeat_t.BLE_HEARTBEAT_tx_bytes = 0x8;
	ble_heartbeat_t.BLE_HEARTBEAT_rx_bytes = 0x7;

	dbc_encode_and_send_BLE_HEARTBEAT(&ble_heartbeat_t);


}

void period_10Hz(uint32_t count)
{

	// eint3_enable_port2(2, eint_rising_edge, receive_Handler);

	// if(xSemaphoreTake(receive_sem,100)){
	//bool flag3=true;
	//bool flag2=true;

		 u2.getChar(&rx_data,0);

		 switch(rx_data){

		 case '0':{
			       //printf("hi \n");          //remove later
			       ble_cmd.BLE_COMM_CMD_enum = {COMM_STOP};
			       LD.setNumber(0);
			 	   break;
		          }
		 case '1':{
			      //printf("bye \n");			//remove later
			      ble_cmd.BLE_COMM_CMD_enum = {COMM_START};
			      LD.setNumber(1);
		          break;
	              }
		 default: break;
		 }

		 dbc_encode_and_send_BLE_COMM_CMD(&ble_cmd);

//    if((LPC_UART2->IIR & 0xE)==ravail)
//	{
//    	NVIC_EnableIRQ(UART2_IRQn);
//	}
//
//    if(receive_data)
//	{
//	uart2_rx_interrupt(&rx_data);
//	printf("%c \n", rx_data);
//	receive_data = 0;
//	}

//    if(rx_data == '1'&& count_1<2){
//	//LE.toggle(1);
//
//	count_1++;
//	//break;
//	}
//
//    if(rx_data == '0'&&count_2<2){
//	//LE.toggle(2);
//	printf("bye \n");
//	ble_cmd.BLE_COMM_CMD_enum = {COMM_STOP};
//	count_2++;
//	//flag2=false;
//	//break;





		//ble_cmd.BLE_COMM_CMD_enum = {COMM_START};
//		ble_chk_pt.BLE_CHCK_PT_lat = 0x1234;
//		ble_chk_pt.BLE_CHCK_PT_long = 0x5678;
//		ble_map_data.BLE_MAP_DATA_dest_lat = 0x1111;
//		ble_map_data.BLE_MAP_DATA_dest_long = 0x2222;
//		ble_map_data.BLE_MAP_DATA_start_lat = 0x3333;
//		ble_map_data.BLE_MAP_DATA_start_long = 0x4444;


	   // dbc_encode_and_send_BLE_HEARTBEAT(&ble_heartbeat_t);



	  // dbc_encode_and_send_BLE_CHCK_PT(&ble_chk_pt);

	    //dbc_encode_and_send_BLE_MAP_DATA(&ble_map_data);


//	while(CAN_rx(can1, &can_msg, 0)){
//
//			dbc_msg_hdr_t can_msg_hdr;
//			can_msg_hdr.dlc = can_msg.frame_fields.data_len;
//			can_msg_hdr.mid = can_msg.msg_id;
//
//			dbc_decode_GEO_LOCATION(&geo_location, can_msg.data.bytes, &can_msg_hdr);
//			dbc_decode_SYSTEM_CMD(&system_cmd, can_msg.data.bytes, &can_msg_hdr);
//		}
//
//
//
//
//	if(dbc_handle_mia_SYSTEM_CMD(&system_cmd, 10)){
//
//			    LD.setNumber(count1);
//				count1++;
//
//				if(count1 == 99)
//					count1 = 0;
//				//LE.toggle(0);
//			}

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
