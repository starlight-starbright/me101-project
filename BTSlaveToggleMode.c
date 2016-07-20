void BTSlaveToggleMode ();

task main()
{
	BTSlaveToggleMode ();
}

void BTSlaveToggleMode ()
{
	int mode = 1;
	while (mode != 3)
	{
		if (mode == 1) //fire
		{
			//sort once
			wait1Msec(2000);
			eraseDisplay();
			displayString (0, "SORT");
		}
		else //mode == 2 sort
		{
			//fire once
			wait1Msec(2000);
			eraseDisplay();
			displayString (0, "FIRE");
		}
		if(bQueuedMsgAvailable())
		{
			ClearMessage();
			mode = messageParm[0];
			displayString(5, "%d", mode);
		}
		wait1Msec (50);
	}
}
