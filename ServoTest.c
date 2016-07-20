#include "NXTServo-lib-UW.c"

#define SENSOR_SERVO S1

void RotateServo (int servo_num, int position1, int position2, int delay_ms);

task main()

{
	SensorType[SENSOR_SERVO] = sensorI2CCustom9V;

	while(nNxtButtonPressed == -1)
	{
		RotateServo (1, -70, 55, 1000);
	}
	while(nNxtButtonPressed != -1){}
	while(nNxtButtonPressed == -1)
	{
		RotateServo (2, -60, 35, 1000);
	}
	while(nNxtButtonPressed != -1){}
}

void RotateServo (int servo_num, int position1, int position2, int delay_ms)
{
	setServoPosition(SENSOR_SERVO, servo_num, position1);
	wait1Msec(delay_ms);
	setServoPosition(SENSOR_SERVO, servo_num, position2);
	wait1Msec(delay_ms);
}
