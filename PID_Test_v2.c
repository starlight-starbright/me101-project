void RotateMotor (int motor_num, int setpoint_degrees, int speed_max);

task main()
{
	while(nNxtButtonPressed == -1)
	{
		RotateMotor (0,90,10);
		wait1Msec(250);
	}
}

void RotateMotor (int motor_num, int setpoint_degrees, int speed_max)
{
	const float kP = 3000.0;
	const float kI = 1.0;
	const float kD = 5.0;
	int integral = 0;
	int error_current = 0;
	int error_last = 0;
	int derivative = 0;
	float power = 0;
	float power_initial = 0;

	nMotorEncoder[motor_num] = 0;

	power_initial = kP * setpoint_degrees;

	while(true) //for (int i = 0; i < 1500; i++)
	{
		error_current = setpoint_degrees - nMotorEncoder[motor_num];
		integral += error_current;
		derivative = error_current - error_last;

		power = kP * error_current + kI * integral + kD * derivative;
		power = speed_max * power / power_initial;

		if (abs(power) < 1 && abs(power) > 0)
		{
			power = 1;
		}

		motor[motor_num] = power;

		error_last = error_current;

		eraseDisplay();
		displayString(0, "%d", nMotorEncoder[motor_num])
		wait1Msec(10);
	}
	motor[motor_num] = 0;
}
