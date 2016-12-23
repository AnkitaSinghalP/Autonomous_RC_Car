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

char *check_ptr_str;
char tx_data[50];

bool checkpoints_ready = false;
int index1 = 0;
bool checkpoint_received_flag = false;


can_msg_t can_msg = { 0 };
BLE_HEARTBEAT_t ble_heartbeat_t = { 0 };
BLE_CHCK_PT_t ble_chk_pt = { 0 };
BLE_COMM_CMD_t ble_cmd = {COMM_STOP};

char rx_data = ' ';

bool n_flag = false;

GEO_LOCATION_t geo_location = { 0 };
GEO_DEST_RCHD_t geo_dest_rchd = {0};
MASTER_SYSTEM_STATUS_t master_system_status = {0};
MASTER_SYSTEM_CMD_t master_system_cmd = { SYSTEM_STOP };
MOTOR_SPEED_t motor_speed = {0};
SENSOR_BATT_t sensor_batt = {0};
SENSOR_ULTRASONIC_t sensor_ultrasonic = {0};

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
	u2.init(115200,255,20);
	CAN_init(can1, 100, 200, 200, 0, 0);
	CAN_bypass_filter_accept_all_msgs();
	CAN_reset_bus(can1);
	LD.setNumber(0);
/*	free(check_ptr_str);*/


	//check_ptr_str = new char [255];

	check_ptr_str = (char*)malloc(201);


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

int number_of_checkpoint = 0, send_checkpoints_count = 0;
bool check_duplicate_zero = false;


void period_1Hz(uint32_t count)
{
	if(CAN_is_bus_off(can1))
		CAN_reset_bus(can1);


	ble_heartbeat_t.BLE_HEARTBEAT_tx_bytes = 0x8;
	ble_heartbeat_t.BLE_HEARTBEAT_rx_bytes = 0x7;

	dbc_encode_and_send_BLE_HEARTBEAT(&ble_heartbeat_t);
	if(checkpoints_ready)
	{
		//printf("received: %s\n",check_ptr_str);

		if(*check_ptr_str == '%')
		{
			number_of_checkpoint = atoi(check_ptr_str + 1);
		}
		//printf("%d\n",number_of_checkpoint);

		if(send_checkpoints_count < number_of_checkpoint)
		{
			check_ptr_str = strchr(check_ptr_str, 'c') + 1;
			ble_chk_pt.BLE_CHCK_PT_lat = atof(check_ptr_str) / 1000000;


			check_ptr_str = strchr(check_ptr_str, 'l') + 3;
			ble_chk_pt.BLE_CHCK_PT_long = atof(check_ptr_str) / 1000000;


			dbc_encode_and_send_BLE_CHCK_PT(&ble_chk_pt);

			//printf("lat = %f   long = %f\n",ble_chk_pt.BLE_CHCK_PT_lat,ble_chk_pt.BLE_CHCK_PT_long);
			send_checkpoints_count ++;
		}
		else
		{
			free(check_ptr_str);
			ble_chk_pt.BLE_CHCK_PT_lat = 0.0;
			ble_chk_pt.BLE_CHCK_PT_long = 0.0;

			if(!check_duplicate_zero)
			{
				//printf("sent last 0\n");
				dbc_encode_and_send_BLE_CHCK_PT(&ble_chk_pt);
			}

			check_duplicate_zero = true;

		}
	}
}


void period_10Hz(uint32_t count)
{
	while(CAN_rx(can1, &can_msg, 0)){
		dbc_msg_hdr_t can_msg_hdr;
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_SENSOR_ULTRASONIC(&sensor_ultrasonic, can_msg.data.bytes, &can_msg_hdr);
		dbc_decode_MASTER_SYSTEM_STATUS(&master_system_status, can_msg.data.bytes, &can_msg_hdr);
		dbc_decode_MASTER_SYSTEM_CMD(&master_system_cmd, can_msg.data.bytes, &can_msg_hdr);
		dbc_decode_MOTOR_SPEED(&motor_speed, can_msg.data.bytes, &can_msg_hdr);
		dbc_decode_GEO_DEST_RCHD(&geo_dest_rchd, can_msg.data.bytes, &can_msg_hdr);
		dbc_decode_SENSOR_BATT(&sensor_batt, can_msg.data.bytes, &can_msg_hdr);
		//dbc_decode_GEO_LOCATION(&geo_location, can_msg.data.bytes, &can_msg_hdr);
	}


//	sensor_ultrasonic.SENSOR_ULTRASONIC_left 			= 1;//
//	sensor_ultrasonic.SENSOR_ULTRASONIC_middle 			= 0;//
//	sensor_ultrasonic.SENSOR_ULTRASONIC_right 			= 1;//
//	sensor_ultrasonic.SENSOR_ULTRASONIC_rear 			= 0;//
//	sensor_ultrasonic.SENSOR_ULTRASONIC_critical 		= 1;//
//	master_system_status.MASTER_SYSTEM_STATUS_ble		= 0;//
//	master_system_status.MASTER_SYSTEM_STATUS_geo		= 0;//
//	master_system_status.MASTER_SYSTEM_STATUS_io		= 0;//
//	master_system_status.MASTER_SYSTEM_STATUS_master	= 0;//
//	master_system_status.MASTER_SYSTEM_STATUS_motor		= 0;//
//	master_system_status.MASTER_SYSTEM_STATUS_sensor	= 0;//
//	master_system_cmd.MASTER_SYSTEM_CMD_enum			= SYSTEM_START;//
//	motor_speed.MOTOR_SPEED_actual						= 99;//
//	geo_dest_rchd.GEO_DEST_RCHD_stat					= 1;//
//	sensor_batt.SENSOR_BATT_stat						= 99;//
	//geo_location.GEO_LOCATION_lat 						= 37.336575;//
	//geo_location.GEO_LOCATION_long 						= -121.882256;//

	if(motor_speed.MOTOR_SPEED_actual > 99)
		motor_speed.MOTOR_SPEED_actual = 99;

	if(sensor_batt.SENSOR_BATT_stat > 99)
		sensor_batt.SENSOR_BATT_stat = 99;

	sprintf(tx_data,"@%d%d%d%d%d,%d%d%d%d%d%d,%d,%d,%d,%d$",
			sensor_ultrasonic.SENSOR_ULTRASONIC_left,
			sensor_ultrasonic.SENSOR_ULTRASONIC_middle,
			sensor_ultrasonic.SENSOR_ULTRASONIC_right,
			sensor_ultrasonic.SENSOR_ULTRASONIC_rear,
			sensor_ultrasonic.SENSOR_ULTRASONIC_critical,
			master_system_status.MASTER_SYSTEM_STATUS_ble,
			master_system_status.MASTER_SYSTEM_STATUS_geo,
			master_system_status.MASTER_SYSTEM_STATUS_io,
			master_system_status.MASTER_SYSTEM_STATUS_master,
			master_system_status.MASTER_SYSTEM_STATUS_motor,
			master_system_status.MASTER_SYSTEM_STATUS_sensor,
			master_system_cmd.MASTER_SYSTEM_CMD_enum,
			(int)motor_speed.MOTOR_SPEED_actual,
			geo_dest_rchd.GEO_DEST_RCHD_stat,
			sensor_batt.SENSOR_BATT_stat);/*,
			geo_location.GEO_LOCATION_lat,
			geo_location.GEO_LOCATION_long);*/

	u2.putline(tx_data,0);

	//printf("%s\n",tx_data);


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

		case '%':
			LD.setNumber(3);
			checkpoint_received_flag = true;
			break;

		default:
			break;
	}

	if(checkpoint_received_flag)
	{
		if(rx_data != '$' && index1 < 200)
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

void period_100Hz(uint32_t count)
{

}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{

}
