#include <heartbeat.hpp>

void heartbeat_rx(void)
{
	dbc_encode_and_send_MASTER_SYSTEM_STATUS(&system_status_message);

	uint16_t can_total_bytes = 0;
	float bus_util_perc = 0;
	/**
	 * todo: do not use a while loop. it is extremely undeterministic. If you need to receive multiple messages
	 * 			or need to process them faster then move this receive function to a faster task.
	 */
	while(CAN_rx(can1, &can_msg, 0))
	{
		dbc_msg_hdr_t can_msg_hdr;
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		if(dbc_decode_BLE_HEARTBEAT(&ble_heartbeat_cmd, can_msg.data.bytes, &can_msg_hdr)){
			system_status_message.MASTER_SYSTEM_STATUS_ble = 1;
			can_total_bytes += ble_heartbeat_cmd.BLE_HEARTBEAT_rx_bytes + ble_heartbeat_cmd.BLE_HEARTBEAT_tx_bytes;
		}
		else if(dbc_handle_mia_BLE_HEARTBEAT(&ble_heartbeat_cmd, 100))
			system_status_message.MASTER_SYSTEM_STATUS_ble = 0;
		if(dbc_decode_SENSOR_HEARTBEAT(&sensor_heartbeat_cmd, can_msg.data.bytes, &can_msg_hdr)){
			system_status_message.MASTER_SYSTEM_STATUS_sensor = 1;
			can_total_bytes += sensor_heartbeat_cmd.SENSOR_HEARTBEAT_rx_bytes + sensor_heartbeat_cmd.SENSOR_HEARTBEAT_tx_bytes;
		}
		else if(dbc_handle_mia_SENSOR_HEARTBEAT(&sensor_heartbeat_cmd, 100))
			system_status_message.MASTER_SYSTEM_STATUS_sensor = 0;
		if(dbc_decode_GEO_HEARTBEAT(&geo_heartbeat_cmd, can_msg.data.bytes, &can_msg_hdr)){
			system_status_message.MASTER_SYSTEM_STATUS_geo = 1;
			can_total_bytes += geo_heartbeat_cmd.GEO_HEARTBEAT_rx_bytes + geo_heartbeat_cmd.GEO_HEARTBEAT_tx_bytes;
		}
		else if(dbc_handle_mia_GEO_HEARTBEAT(&geo_heartbeat_cmd, 100))
			system_status_message.MASTER_SYSTEM_STATUS_geo = 0;
		if(dbc_decode_MOTOR_HEARTBEAT(&motor_heartbeat_cmd, can_msg.data.bytes, &can_msg_hdr)){
			system_status_message.MASTER_SYSTEM_STATUS_motor = 1;
			can_total_bytes += motor_heartbeat_cmd.MOTOR_HEARTBEAT_rx_bytes + motor_heartbeat_cmd.MOTOR_HEARTBEAT_tx_bytes;
		}
		else if(dbc_handle_mia_MOTOR_HEARTBEAT(&motor_heartbeat_cmd, 100))
			system_status_message.MASTER_SYSTEM_STATUS_motor = 0;
		if(dbc_decode_IO_HEARTBEAT(&io_heartbeat_cmd, can_msg.data.bytes, &can_msg_hdr)){
			system_status_message.MASTER_SYSTEM_STATUS_io = 1;
			can_total_bytes += io_heartbeat_cmd.IO_HEARTBEAT_rx_bytes + io_heartbeat_cmd.IO_HEARTBEAT_tx_bytes;
		}
		else if(dbc_handle_mia_IO_HEARTBEAT(&io_heartbeat_cmd, 100))
			system_status_message.MASTER_SYSTEM_STATUS_io = 0;
	}
	/**
	 * todo: what is this supposed to represent? Total bus load? Why are you dividing by 1000?
	 * 			If this is in bytes then at 100kbps you can get ~8KB/s of data. Use BusMaster to analyze
	 * 			bus traffic instead
	 */
	 /*
	  * Calculate bus utilization per sec in percentage to be displayed on LCD. total_bytes * 100 / 100KB
	  */
	bus_util_perc = (can_total_bytes)/(1000);
	system_status_message.MASTER_SYSTEM_STATUS_util = bus_util_perc;
	dbc_encode_and_send_MASTER_SYSTEM_STATUS(&system_status_message);

}
