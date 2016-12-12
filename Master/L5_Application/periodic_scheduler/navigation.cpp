#include "navigation.hpp"
#include "free_run.hpp"

void navigation_mode(void)
{
	if((!sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_critical) && (!geo_dest_rchd_cmd.GEO_DEST_RCHD_stat))
		{
			switch (geo_direction_cmd.GEO_DIRECTION_enum)
			{
			case(DIR_FORWARD):
				if(!sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle)
				{
					motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_FORWARD;
					motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
					LD.setNumber(DIR_FORWARD);
				}
				else
				{
					free_run_func();
				}
				break;

			case(DIR_LEFT):
				if(!sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left)
				{
					motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_LEFT;
					motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
					LD.setNumber(DIR_LEFT);
				}
				else
				{
					free_run_func();
				}
				break;

			case(DIR_RIGHT):
				if(!sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right)
				{
					motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_RIGHT;
					motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
					LD.setNumber(DIR_RIGHT);
				}
				else
				{
					free_run_func();
				}
				break;

			case(DIR_HALF_LEFT):
				if(!sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_left && !sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle)
				{
					motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_HALF_LEFT;
					motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
					LD.setNumber(DIR_HALF_LEFT);

				}
				else
				{
					free_run_func();
				}
				break;

			case(DIR_HALF_RIGHT):
				if(!sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_right && !sensor_ultrasonic_cmd.SENSOR_ULTRASONIC_middle)
				{
					motor_cmd_message.MASTER_MOTOR_CMD_steer = STEER_HALF_RIGHT;
					motor_cmd_message.MASTER_MOTOR_CMD_drive = START;
					LD.setNumber(DIR_HALF_RIGHT);

				}
				else
				{
					free_run_func();
				}
				break;

			case(DIR_REVERSE):
			default:
				motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
				LD.setNumber(DIR_REVERSE);

				break;
			}
		} else {
			motor_cmd_message.MASTER_MOTOR_CMD_drive = STOP;
			LD.setNumber(77);
		}
}
