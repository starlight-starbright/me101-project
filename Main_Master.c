int preferences[5] = {0, 0, 0, 0, 0};
string color_names[5]={"PURPLE", "ORANGE", "GREEN", "YELLOW", "RED"};

void BTMasterCalibrate(int motor_num);
void GetPreferences ();
bool ReadbackPreferences ();
void BTMasterSendPreferences ();
void BTMasterStartOperation ();
void BTMasterToggleMode ();

task main()
{
	//calibrate slave motors
	BTMasterCalibrate(0);
	BTMasterCalibrate(1);

	//get preferences from user
	do
	{
		GetPreferences ();
	} while (!ReadbackPreferences ());

	//send preferences
	BTMasterSendPreferences ();

	//send start signal
	BTMasterStartOperation ();

	//main operation, toggle between modes
	BTMasterToggleMode ();
	while (true){}

	//receive logs
	//BTMasterGetLogs ();

	//display logs
	//DisplayLogs ();

}

void BTMasterCalibrate(int motor_num)
{
	int button = 0;

	eraseDisplay();
	displayString(0, "Do you want");
	displayString(1, "to calibrate");
	displayString(2, "MOTOR %d?", motor_num);
	displayString(7, "NO           YES");

	while (nNxtButtonPressed == -1){}
	button = nNxtButtonPressed;
	while(nNxtButtonPressed!=-1){}

	if (button == 2) //no
	{
		sendMessage(0);
		wait1Msec(100);
		return;
	}
	else if (button == 1)//yes
	{
		sendMessage(1);
		wait1Msec(100);
	}

  eraseDisplay();
  displayString(7, "LEFT   OK   RIGHT");

	while(nNxtButtonPressed == -1){}
	while(nNxtButtonPressed != 3)
	{
		sendMessage(nNxtButtonPressed);
		wait1Msec(50);
	}
	while(nNxtButtonPressed == 3){}
	ClearMessage();
	sendMessage(3);
}

void GetPreferences()
{
	for(int i = 0; i < 5 ; i++)
	{
		eraseDisplay();
		displayString(0, "Do you want");
		displayString(1, "the color");
		displayString(2, "%s?", color_names[i]);
		displayString(7, "NO           YES");
		while (nNxtButtonPressed == -1){}
		if (nNxtButtonPressed == 2) //no
		{
			preferences[i]= 0;
		}
		else if (nNxtButtonPressed == 1) //yes
		{
			preferences[i]=1;
		}
		while (nNxtButtonPressed != -1){}
	}
	eraseDisplay();
	displayString(0, "Do you want");
	displayString(1, "random color");
	displayString(2, "launching?");
	displayString(7, "NO          YES");
	while (nNxtButtonPressed == -1){}
	if (nNxtButtonPressed == 1) //yes
	{
		while (nNxtButtonPressed != -1){}
		return;
	}
	while (nNxtButtonPressed != -1){}

	//no
	for(int i = 0; i < 5 ; i++)
	{
		if (preferences[i] != 0)
		{
			eraseDisplay();
			displayString(0, "Enter priority");
			displayString(1, "for color:");
			displayString(2, "%s", color_names[i]);
			displayString(7, "HIGH   MED    LOW");
			while (nNxtButtonPressed == -1){}
			if (nNxtButtonPressed == 2) //high
			{
				preferences[i] = 1;
			}
			else if (nNxtButtonPressed == 3) //med
			{
				preferences[i] = 2;
			}
			else //low
			{
				preferences[i] = 3;
			}
			while (nNxtButtonPressed != -1){}
		}
	}
}

bool ReadbackPreferences()
{
	int counter = 0;
	int button = 0;

	eraseDisplay();
	displayString(0, "You selected:");
	for (int i = 0; i < 5; i++)
	{
		if (preferences[i] != 0)
		{
				displayString(counter+1, "%s - %d", color_names[i], preferences[i]);
				counter++;
		}
	}
	if (counter == 0)
	{
		displayString(2, "WARNING");
		displayString(3, "NO COLORS");
	}
	displayString(7, "BACK        OKAY");

	while(nNxtButtonPressed == -1){}
	button = nNxtButtonPressed;
	while(nNxtButtonPressed != -1){}
	if (button == 2) //back
	{
		return false;
	}
	else if (button == 1) //okay
	{
		return true;
	}
	return false;
}

void BTMasterSendPreferences ()
{
	int code = 0;
	for (int i = 0; i < 5; i++)
	{
		code += preferences[i] * (10000 / pow(10, i));
	}
	sendMessage (code);
}

void BTMasterStartOperation()
{
	eraseDisplay();
	displayString(0, "Press any button");
	displayString(1, "to begin sorting");

	while(nNxtButtonPressed == -1){}
	while(nNxtButtonPressed != -1){}

	eraseDisplay();
	sendMessage(1);
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
		//ClearMessage();
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
			sendMessage (mode);
		}
		eraseDisplay();
		displayString (0, "%d", mode);
	} while (mode != 0);

	wait1Msec (1000);
}
