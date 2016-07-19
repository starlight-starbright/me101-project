void BTSlaveCalibrate (int motor_num);

task main()
{
	BTSlaveCalibrate(0);
	BTSlaveCalibrate(1);
}

void BTSlaveCalibrate (int motor_num)
{
	bool proceed = true;
	int button = 0;

	while(!bQueuedMsgAvailable()){}
	while(bQueuedMsgAvailable())
	{
		ClearMessage();
		proceed = (bool)(messageParm[0]);
		eraseDisplay();
		displayString(0, "MOTOR %d", motor_num);
		wait1Msec(50);
	}
	if (proceed)
	{
		while(!bQueuedMsgAvailable()){}
		while(true)
		{
			ClearMessage();
			button = messageParm[0];
			eraseDisplay();
			displayString (1, "%d", button);
			if (button == 1)
			{
				motor[motor_num] = 10;
				while (button == 1)
				{
					ClearMessage();
					button = messageParm[0];
					wait1Msec(30);
				}
			}
			else if (button == 2)
			{
				motor[motor_num] = -10;
				while (button == 2)
				{
					ClearMessage();
					button = messageParm[0];
					wait1Msec(30);
				}
			}
			else if (button == 3)
			{
				motor[motor_num] = 0;
				return;
			}
			motor[motor_num] = 0;
			wait1Msec(50);
		}
	}
}
/*
void BTCalibrateSlave(int motor_num)
{
  while (nNxtButtonPressed != 3)
  {
    if (nNxtButtonPressed == 2) //left
    {
    	motor[motor_num] = -5;
    	while (nNxtButtonPressed == 2){}
    }
    else if (nNxtButtonPressed == 1) //right
    {
    	motor[motor_num] = 5;
    	while (nNxtButtonPressed == 1){}
    }
    motor[motor_num] = 0;
  }
	while (nNxtButtonPressed == 3){}
}
*/
