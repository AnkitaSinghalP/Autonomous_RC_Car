#include "free_run.hpp"

void free_run_func()
{
	while(CAN_rx(can1, &can_msg, 0))
	{
		dbc_msg_hdr_t can_msg_fr;
		can_msg_fr.dlc = can_msg.frame_fields.data_len;
		can_msg_fr.mid = can_msg.msg_id;

		dbc_decode_SENSOR_ULTRASONIC(&sensor_ultrasonic_cmd, can_msg.data.bytes, &can_msg_fr);
		if(sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle)
		{
			//	motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_LEFT;
			//	motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
			motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
			LD.setNumber(8);
		}

		else if(!sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left && sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle && sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right)
		{
			//	motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_LEFT;
			//	motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
			motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
			LD.setNumber(8);

		}

		else if(sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left && sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right)
		{
			motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
			LD.setNumber(8);

		}

		else if(sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left && sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle && !sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right)
		{
			//	motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_RIGHT;
			//	motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
			motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
			LD.setNumber(8);
		}

		else if(sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left && sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle && sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right)
		{
			motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
			LD.setNumber(8);

		}

		else if(!sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left && !sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle && !sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right)
		{
			motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_FORWARD;
			motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
			LD.setNumber(0);

		}

		else if(!sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left && !sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle && sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right)
		{
			motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_HALF_LEFT;
			motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
			LD.setNumber(1);

		}

		else if(sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left && !sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle && !sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right)
		{
			motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_HALF_RIGHT;
			motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
			LD.setNumber(2);
		}


		if(!sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left && !sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle && sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right)
		{
			motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
			LE.on(4);
		}

	dbc_encode_and_send_MASTER_MOTOR_CMD(&motor_cmd_message);
	}
}
