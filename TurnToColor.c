void RotateMotor (int motor_num, int degrees, int speed_max);
int TurnToColor (int chamber_requested, int chamber_current, int side);

task main()
{
	int x = 0;
	x = TurnToColor (5, x, 0);
	wait1Msec(250);
	x = TurnToColor (0, x, 0);

}

void RotateMotor (int motor_num, int degrees, int speed_max)
{
	const int kP = 150; //actual constants are multiplied by 100
	const int kI = 0;
	const int kD = 0;
	int integral;
	int error_current;
	int error_last;
	int derivative;
	int power;

	nMotorEncoder[motor_num] = 0;

	for (int i = 0; i < 100; i++)
	{
		error_current = degrees - nMotorEncoder[motor_num];
		integral += error_current;
		derivative = error_current - error_last;

		power = kP * error_current + kI * integral + kD * derivative;
		power /= 100;

		if (abs(power) > speed_max)
		{
			power = power / abs(power) * speed_max;
		}

		motor[motor_num] = power;
		error_last = error_current;
		wait1Msec(10);
	}
	motor[motor_num] = 0;
}

//Turns to a requested chamber and returns the new chamber number
int TurnToColor (int chamber_requested, int chamber_current, int side)
{
	int delta = 0;
	int degrees = 0;

	if (side == 1)
	{
		chamber_requested = (chamber_requested + 3) % 6;
	}

	if (chamber_requested != chamber_current)
	{
		if (chamber_requested)
			RotateMotor (1, degrees, 15);
	}
	return chamber_requested;
}
