void BTMasterToggleMode ();

task main()
{
	BTMasterToggleMode ();
}

void BTMasterToggleMode ()
{
	int button = 3;
	int mode = 1;
	do
	{
		while(nNxtButtonPressed == -1)
		{
			ClearMessage ();
			sendMessage (mode);
			wait1Msec (50);
		}
		button = nNxtButtonPressed;
		while(nNxtButtonPressed != -1){}
		ClearMessage();
		if (button == 2) //sort
		{
			mode = 1;
		}
		else if (button == 1) //fire
		{
			mode = -1;
		}
		else //quit
		{
			mode = 0;
		}
		eraseDisplay();
		displayString (0, "%d", mode);
	} while (mode != 0);

	wait1Msec (1000);
}
