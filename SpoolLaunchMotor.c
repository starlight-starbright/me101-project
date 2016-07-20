void SpoolLaunchMotor (int motor_num, bool spool_up, int power_max);

task main()
{
	SpoolLaunchMotor (2, true, -70);
	while (nNxtButtonPressed == -1){}
}

void SpoolLaunchMotor (int motor_num, bool spool_up, int power_max)
{
	if (spool_up)
	{
		time1[1] = 0;
		while(time1[1] <= 1000)
		{
			motor[motorC] = (int)(power_max * time1[0] / 1000.0);
			wait1Msec(25);
		}
	}
	else
	{
		time1[1] = 0;
		while(time1[1] <= 1000)
		{
			motor[motor_num] = (int)(1.0 * power_max * (-time1[0] + 1000));
			wait1Msec(25);
		}
	}
}
