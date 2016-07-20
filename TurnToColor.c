void RotateMotor (int motor_num, int degrees, int speed_max);
void RotateMotorSimple (int motor_num, int degrees, int speed);
int TurnToColor (int chamber_requested, int chamber_current, int side);

task main()
{
	int x = 0;
	x = TurnToColor (5, x, 0);
	wait1Msec(250);
	x = TurnToColor (2, x, 0);
	wait1Msec(250);
	x = TurnToColor (3, x, 1);
	wait1Msec(250);
	x = TurnToColor (0, x, 1);

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

void RotateMotorSimple (int motor_num, int degrees, int speed)
{
	nMotorEncoder[motor_num] = 0;
	motor[motor_num] = degrees/abs(degrees) * speed;
	while (abs(nMotorEncoder[motor_num]) < abs(degrees)){}
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
		delta = abs(chamber_requested - chamber_current);
		if (delta > 3)
		{
			delta = 6 - delta;
		}
		degrees = (60 * delta);
		RotateMotorSimple (1, degrees, 5);
	}
	return chamber_requested;
}
