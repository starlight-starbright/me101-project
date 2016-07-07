#define MOTOR_NUM 2

task main()
{
	int power = 0;
	bool exit = false;

	displayString(0, "Configure power");
	while (nNxtButtonPressed == -1){}
	eraseDisplay();
	while (exit == false)
	{
		while (nNxtButtonPressed == -1){}
		if (nNxtButtonPressed == 2) //left button
		{
			power -= 5;
		}
		else if (nNxtButtonPressed == 1) //right button
		{
			power += 5;
		}
		else //center button
		{
			exit = true;
		}
		while (nNxtButtonPressed != -1){}
		eraseDisplay();
		displayString(0, "Power: %d", power);
	}

	displayString(1, "Press to spool");

	while(true)
	{
		while (nNxtButtonPressed != 3){}
		for (int i = 0; i < power; i++)
		{
			motor[MOTOR_NUM] = i;
			wait1Msec(10);
		}
		while (nNxtButtonPressed == -1){}
		motor[MOTOR_NUM] = 0;
		while (nNxtButtonPressed != -1){}
	}
}
