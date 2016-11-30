#include "system_cmd.hpp"
#include "free_run.hpp"

void sys_cmd(void)
{
	static bool sys_start_flag = 0;
	while(CAN_rx(can1, &can_msg, 0))
	{
		dbc_msg_hdr_t can_msg_hdr;
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_BLE_COMM_CMD(&ble_comm_cmd, can_msg.data.bytes, &can_msg_hdr);
		dbc_decode_SENSOR_ULTRASONIC(&sensor_ultrasonic_cmd, can_msg.data.bytes, &can_msg_hdr);
		if(dbc_decode_BLE_HEARTBEAT(&ble_heartbeat_cmd, can_msg.data.bytes, &can_msg_hdr))
			system_status_message.MASTER_SYSTEM_STATUS_ble = 1;
		if(dbc_decode_SENSOR_HEARTBEAT(&sensor_heartbeat_cmd, can_msg.data.bytes, &can_msg_hdr))
			system_status_message.MASTER_SYSTEM_STATUS_sensor = 1;
		if(dbc_decode_GEO_HEARTBEAT(&geo_heartbeat_cmd, can_msg.data.bytes, &can_msg_hdr))
			system_status_message.MASTER_SYSTEM_STATUS_geo = 1;
		if (dbc_decode_IO_HEARTBEAT(&io_heartbeat_cmd, can_msg.data.bytes, &can_msg_hdr))
			system_status_message.MASTER_SYSTEM_STATUS_io = 1;
		if(dbc_decode_MOTOR_HEARTBEAT(&motor_heartbeat_cmd, can_msg.data.bytes, &can_msg_hdr))
			system_status_message.MASTER_SYSTEM_STATUS_motor = 1;

		// initial start/stop command from BLE, will wake all the modules.
		switch (ble_comm_cmd.BLE_COMM_CMD_enum)
		{
			case(COMM_STOP):
					system_cmd_message.MASTER_SYSTEM_CMD_enum = SYSTEM_STOP;
					dbc_encode_and_send_MASTER_SYSTEM_CMD(&system_cmd_message);
					sys_start_flag = 0;
					LD.setNumber(55);
					break;

			case(COMM_START):
					system_cmd_message.MASTER_SYSTEM_CMD_enum = SYSTEM_START;
					dbc_encode_and_send_MASTER_SYSTEM_CMD(&system_cmd_message);
					sys_start_flag = 1;
					LD.setNumber(44);
					//free_run_func();
					break;

			case(COMM_RESET):
					system_cmd_message.MASTER_SYSTEM_CMD_enum = SYSTEM_RESET;
					break;

			default:
					system_cmd_message.MASTER_SYSTEM_CMD_enum = SYSTEM_STOP;
					dbc_encode_and_send_MASTER_SYSTEM_CMD(&system_cmd_message);
					break;
		}
	}

	if (sys_start_flag)
		free_run_func();
}
