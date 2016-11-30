#include <heartbeat.hpp>

void heartbeat_rx(void)
{

//	uint16_t can_total_bytes = 0;
//	float bus_util_perc = 0;

	if(dbc_handle_mia_MOTOR_HEARTBEAT(&motor_heartbeat_cmd, 100))
		system_status_message.MASTER_SYSTEM_STATUS_motor = 0;
	if(dbc_handle_mia_BLE_HEARTBEAT(&ble_heartbeat_cmd, 100))
		system_status_message.MASTER_SYSTEM_STATUS_ble = 0;
	if(dbc_handle_mia_SENSOR_HEARTBEAT(&sensor_heartbeat_cmd, 100))
		system_status_message.MASTER_SYSTEM_STATUS_sensor = 0;
	if(dbc_handle_mia_GEO_HEARTBEAT(&geo_heartbeat_cmd, 100))
		system_status_message.MASTER_SYSTEM_STATUS_geo = 0;
	if(dbc_handle_mia_IO_HEARTBEAT(&io_heartbeat_cmd, 100))
		system_status_message.MASTER_SYSTEM_STATUS_io = 0;

	system_status_message.MASTER_SYSTEM_STATUS_master = 1;
	 /*
	  * Calculate bus utilization per sec in percentage to be displayed on LCD. total_bytes * 100 / 100KB
	  */

//	bus_util_perc = (can_total_bytes)/(1000);
//	system_status_message.MASTER_SYSTEM_STATUS_util = bus_util_perc;

	dbc_encode_and_send_MASTER_SYSTEM_STATUS(&system_status_message);

}
