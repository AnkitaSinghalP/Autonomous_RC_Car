#include "free_run.hpp"

void free_run_func()
{
	static uint8_t obstacle_data;

	obstacle_data= (sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right << 2 ) | (sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle << 1) |(sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left);

	/*
	 * The below switch case is based on the lookup table of the obstacle detection sensors
	 * R | M | L
	 * 0 | 0 | 0	0: Forward
	 * 0 | 0 | 1	1: Half Right
	 * 0 | 1 | 0	2: Left
	 * 0 | 1 | 1	3: Right
	 * 1 | 0 | 0	4: Half Left
	 * 1 | 0 | 1	5: Forward
	 * 1 | 1 | 0	6: Left
	 * 1 | 1 | 1	7: Stop
	 */

	if(!sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_critical)
	{
		switch(obstacle_data)
		{
		case 0:
		case 5:
			motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_FORWARD;
			motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
			LD.setNumber(44);
			break;

		case 1:
			motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_HALF_RIGHT;
			motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
			LD.setNumber(33);
			break;

		case 2:
		case 6:
			motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_LEFT;
			motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
			LD.setNumber(00);
			break;

		case 3:
			motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_RIGHT;
			motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
			LD.setNumber(22);
			break;

		case 4:
			motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_HALF_LEFT;
			motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
			LD.setNumber(11);
			break;

		case 7:
			motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
			LD.setNumber(55);
			break;

		default:
			motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
			LD.setNumber(55);

		}
	} else {
		motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
		LD.setNumber(77);
	}
}

