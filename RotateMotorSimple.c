void RotateMotorSimple (int motor_num, int degrees, int speed);

task main()
{
	while (true)
	{
		RotateMotorSimple(1,60,5);
		wait1Msec(250);
	}
}

void RotateMotorSimple (int motor_num, int degrees, int speed)
{
	nMotorEncoder[motor_num] = 0;
	motor[motor_num] = degrees/abs(degrees) * speed;
	while (abs(nMotorEncoder[motor_num]) < abs(degrees) - 1){}
	motor[motor_num] = 0;
}
