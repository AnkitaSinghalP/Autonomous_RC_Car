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
#include <inttypes.h>
#include "math.h"
#include "file_logger.h"
#include "queue.h"

volatile int receive_data = 0;

bool flag1;
can_std_id_t id;
bool flag_tx;
can_msg_t abc;

char *check_ptr_str;
bool checkpoints_ready = false;
int index1 = 0;
bool checkpoint_received_flag = false;


MASTER_SYSTEM_CMD_enum_E system_cmd = {SYSTEM_STOP};
GEO_LOCATION_t geo_location = { 0 };

can_msg_t can_msg = { 0 };
MASTER_SYSTEM_STATUS_t master_system_status;

BLE_HEARTBEAT_t ble_heartbeat_t = { 0 };
BLE_CHCK_PT_t ble_chk_pt = { 0 };
BLE_COMM_CMD_t ble_cmd = {COMM_STOP};

SemaphoreHandle_t receive_sem;

char rx_data;

char delimiter1 = '-';
char delimiter = '#';
char com_delimiter = ',';
char ble_limiter = 'b';
char io_limiter = 'i';
char sen_limiter = 's';
char mas_limiter = 'm';
char mot_limiter = 'l';


bool n_flag = false;

char tx_data[23];

const uint32_t SYSTEM_CMD__MIA_MS = 1000;
const MASTER_SYSTEM_CMD_enum_E SYSTEM_CMD__MIA_MSG = { SYSTEM_STOP };

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

bool period_init(void)
{
	u2.init(115200,255,50);
	CAN_init(can1, 100, 20, 20, 0, 0);
	CAN_bypass_filter_accept_all_msgs();
	CAN_reset_bus(can1);
	LD.setNumber(0);

	check_ptr_str = new char [255];
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
	while(CAN_rx(can1, &can_msg, 0)){
		dbc_msg_hdr_t can_msg_hdr;
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		//dbc_decode_GEO_LOCATION()
	}
//	if(!n_flag)
//	{

		float lat = 37.336837;
		float lng = -121.881508;

		tx_data[0] = '-';

		sprintf(tx_data+1,"%f",lat);

		tx_data[10] = ',';

		sprintf(tx_data+11,"%f",lng);

		tx_data[22] = '#';

		//printf("%s\n",tx_data);

		u2.putline(tx_data,0);
//	}


	if(checkpoints_ready)
	{
		check_ptr_str = strchr(check_ptr_str, 'c') + 1;
		ble_chk_pt.BLE_CHCK_PT_lat = atof(check_ptr_str) / 1000000;


		check_ptr_str = strchr(check_ptr_str, 'l') + 3;
		ble_chk_pt.BLE_CHCK_PT_long = 0 - atof(check_ptr_str) / 1000000;

		dbc_encode_and_send_BLE_CHCK_PT(&ble_chk_pt);

		//printf("lat = %f   long = %f\n",ble_chk_pt.BLE_CHCK_PT_lat,ble_chk_pt.BLE_CHCK_PT_long);

	}


}

void period_100Hz(uint32_t count)
{


	u2.getChar(&rx_data,0);

	switch(rx_data){

		case 'a':
			checkpoint_received_flag = false;
			ble_cmd.BLE_COMM_CMD_enum = {COMM_STOP};
			dbc_encode_and_send_BLE_COMM_CMD(&ble_cmd);
			LD.setNumber(0);
			n_flag = false;
			break;

		case 'b':
			checkpoint_received_flag = false;
			ble_cmd.BLE_COMM_CMD_enum = {COMM_START};
			dbc_encode_and_send_BLE_COMM_CMD(&ble_cmd);
			LD.setNumber(1);
			n_flag = true;
			break;

		case 'c':
			LD.setNumber(3);
			checkpoint_received_flag = true;
			break;

		default:
			break;
	}

	if(checkpoint_received_flag)
	{
		if(rx_data != '$')
		{

			check_ptr_str[index1] = rx_data;
			index1++;
		}
		else
		{
			checkpoint_received_flag = false;
			checkpoints_ready = true;
		}
	}

}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{

}
