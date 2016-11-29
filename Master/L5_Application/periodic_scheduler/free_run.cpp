#include "free_run.hpp"

void free_run_func()
{
	while(CAN_rx(can1, &can_msg, 0))
	{
		dbc_msg_hdr_t can_msg_fr;
		can_msg_fr.dlc = can_msg.frame_fields.data_len;
		can_msg_fr.mid = can_msg.msg_id;

		dbc_decode_SENSOR_ULTRASONIC(&sensor_ultrasonic_cmd, can_msg.data.bytes, &can_msg_fr);
		uint8_t obstacle_data= (sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right << 2 ) | (sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle << 1) |(sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left);
		/*
		 * The below switch case is based on the lookup table of the obstacle detection sensors
		 * R | M | L
		 * 0 | 0 | 0	Forward
		 * 0 | 0 | 1	Half Right
		 * 0 | 1 | 0	Left
		 * 0 | 1 | 1	Right
		 * 1 | 0 | 0	Half Left
		 * 1 | 0 | 1	Forward
		 * 1 | 1 | 0	Left
		 * 1 | 1 | 1	Stop
		 */
		if(sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_critical == 1)
		{
			motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
			LD.setNumber(88);
		}
		else
		{
			switch(obstacle_data)
			{
			case 0:
			case 5:
				motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_FORWARD;
				motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
				LD.setNumber(2);
				break;

			case 1:
				motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_HALF_RIGHT;
				motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
				LD.setNumber(1);
				break;

			case 2:
			case 6:
				motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_LEFT;
				motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
				LD.setNumber(3);
				break;

			case 3:
				motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_RIGHT;
				motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
				LD.setNumber(1);
				break;

			case 4:
				motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_HALF_LEFT;
				motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
				LD.setNumber(3);
				break;

			case 7:
				motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
				LD.setNumber(4);
				break;

			default:
				motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
				LD.setNumber(77);

			}
		}
		dbc_encode_and_send_MASTER_MOTOR_CMD(&motor_cmd_message);
		}
	}
//	/**
//	 * todo: do not use a while loop
//	 */
//	while(CAN_rx(can1, &can_msg, 0))
//	{
//		dbc_msg_hdr_t can_msg_fr;
//		can_msg_fr.dlc = can_msg.frame_fields.data_len;
//		can_msg_fr.mid = can_msg.msg_id;
//
//		dbc_decode_SENSOR_ULTRASONIC(&sensor_ultrasonic_cmd, can_msg.data.bytes, &can_msg_fr);
//		if(sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle)
//		{
//			//	motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_LEFT;
//			//	motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
//			motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
//			LD.setNumber(8);
//		}
//
//		else if(!sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left && sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle && sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right)
//		{
//			//	motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_LEFT;
//			//	motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
//			motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
//			LD.setNumber(8);
//
//		}
//
//		else if(sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left && sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right)
//		{
//			motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
//			LD.setNumber(8);
//
//		}
//
//		else if(sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left && sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle && !sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right)
//		{
//			//	motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_RIGHT;
//			//	motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
//			motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
//			LD.setNumber(8);
//		}
//
//		else if(sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left && sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle && sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right)
//		{
//			motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
//			LD.setNumber(8);
//
//		}
//
//		else if(!sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left && !sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle && !sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right)
//		{
//			motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_FORWARD;
//			motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
//			LD.setNumber(0);
//
//		}
//
//		else if(!sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left && !sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle && sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right)
//		{
//			motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_HALF_LEFT;
//			motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
//			LD.setNumber(1);
//
//		}
//
//		else if(sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left && !sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle && !sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right)
//		{
//			motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_HALF_RIGHT;
//			motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
//			LD.setNumber(2);
//		}
//
//		/**
//		 * todo: you already have this case on line 62 above. why leave this as an additional if statement?
//		 */
//		if(!sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left && !sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle && sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right)
//		{
//			motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
//			LE.on(4);
//		}
//
//	}
//}
