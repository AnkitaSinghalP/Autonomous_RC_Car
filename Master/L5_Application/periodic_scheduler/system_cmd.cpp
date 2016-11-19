#include <system_cmd.hpp>
#include "free_run.hpp"

void sys_cmd(void)
{
	while(CAN_rx(can1, &can_msg, 0))
	{
		dbc_msg_hdr_t can_msg_hdr;
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		dbc_decode_BLE_COMM_CMD(&ble_comm_cmd, can_msg.data.bytes, &can_msg_hdr);

		// initial start/stop command from BLE, will wake all the modules.
		switch (ble_comm_cmd.BLE_COMM_CMD_enum)
		{
			case(COMM_STOP):
					system_cmd_message.MASTER_SYSTEM_CMD_enum = SYSTEM_STOP;
					dbc_encode_and_send_MASTER_SYSTEM_CMD(&system_cmd_message);
					break;

			case(COMM_START):
					system_cmd_message.MASTER_SYSTEM_CMD_enum = SYSTEM_START;
					dbc_encode_and_send_MASTER_SYSTEM_CMD(&system_cmd_message);
					free_run_func();
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
}
