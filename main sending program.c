
int colourchoice[5]={0,0,0,0,0};
string colours[5]={"Purple", "Orange", "Green", "Yellow", "Red"};

////////////////////////////////////bt calibrating function//////////////////////////////////////
void BTMasterCalibrate(int motor_number)
{
	int button_pressed=-1;

	eraseDisplay();
	displayString(0, "Do you want to");
	displayString(1,"calibrate ?");
	displayString(2, "(orange=yes)");
	displayString(3, "motor(""%d"")",motor_number);

	while (nNxtButtonPressed == -1){}
	button_pressed=nNxtButtonPressed;
	while(nNxtButtonPressed!=-1){}

	if(button_pressed==3)
	{
		displayString(5, "orange SUCCESS");
		wait1Msec(500);
		sendMessage(1);
		wait1Msec(100);
	}

	else
	{
		sendMessage(0);
		wait1Msec(100);
		return;
	}
  eraseDisplay();
	while(nNxtButtonPressed==-1){}
	while(nNxtButtonPressed!=3)
	{
		sendMessage(nNxtButtonPressed);
		eraseDisplay();
		displayString(0, "%d", nNxtButtonPressed);
		wait1Msec(50);
		displayString(2, "orange exits");
	}
	while(nNxtButtonPressed == 3){}
	ClearMessage();
	sendMessage(3);
}


//function to send start signal to robot/////////////////////////////////////////////////
void BTMasterStartOperation()
{
	eraseDisplay();
	displayString(0, "Press any button");
	displayString(1,"to start robot");

	while(nNxtButtonPressed == -1){}

	sendMessage(1);
	return;

}

//function gets color preferences//////////////////////////////////////////////
int inputcolours()
{
	int decision[5] = {0, 0, 0, 0, 0};
	int final = 0;
	//string colours[5]={"Purple", "Orange", "Green", "Yellow", "Red"};

	for(int i = 0; i < 5 ; i++)
	{
		eraseDisplay();
		displayString(2, "Would you like ");
		displayString(4,"the color below ?");
		displayString(6, "%s", colours[i]);

		while (nNxtButtonPressed == -1){}
		if (nNxtButtonPressed == 3)
		{
			decision[i]=1;
		}
		else
		{
			decision[i]=0;
		}
		colourchoice[i]=decision[i];
		while (nNxtButtonPressed != -1){}
	}
	final=decision[0]*10000+decision[1]*1000+decision[2]*100+decision[3]*10+decision[4];
	return final;
}

//function gets whether random or ordered delivery////////////////////////////////////////
bool inputstyle()
{
	int buttonval=0;
			eraseDisplay();
			displayString(0, "Would you like ");
			displayString(1,"the colors");
			displayString(2, "randomly ?");
			displayString(3, "(orange for yes)");

			while(nNxtButtonPressed==-1){}
			buttonval=nNxtButtonPressed;
			while(nNxtButtonPressed!=-1){}
			if(buttonval==3)
			{
				return true;
			}
			else
			{
				return false;
			}

}

//function takes in order or priority of delivery from 1-3////////////////////
int inputorder()
{
	int decision[5] = {0, 0, 0, 0, 0};
	int final = 0;
	//string colours[5]={"Purple", "Orange", "Green", "Yellow", "Red"};

	for(int i = 0; i < 5 ; i++)
	{
		if(colourchoice[i]!=0)
		{
			eraseDisplay();
			displayString(0, "When do you want ");
			displayString(1,"the color below ?");
			displayString(5, "%s", colours[i]);

			while (nNxtButtonPressed == -1){}
			if (nNxtButtonPressed == 3)//orange, sets to 3rd
			{
				decision[i]=3;
			}
			else if (nNxtButtonPressed == 2)//left sets to 2nd
			{
				decision[i]=2;
			}
			else//right sets to first
			{
				decision[i]=1;
			}

			while (nNxtButtonPressed != -1){}
		}
	}
	final=decision[0]*10000+decision[1]*1000+decision[2]*100+decision[3]*10+decision[4];
	return final;
}

///////////////////////////Display Choice/////////////////////////////////////
void DisplayChoice()
{
	eraseDisplay();
	displayString(0, "You selected the");
	displayString(1, "following colours:");

	int a=0;
	for(int i=0; i<5;i++)
	{
		if(colourchoice[i]==1)
		{
				displayString(a+3, "%s", colours[i]);
				a++;
		}

		else if(i==4 && a==0)
		{
			displayString(4,"No colours");
		}

	}
		wait1Msec(2000);
		displayString(2, "orange for next");
		while(nNxtButtonPressed==-1){};
}

//////////////////////////////////////////////////////////BT Master Toggle//////////////////////////
void BTMasterToggle()
{
	int x = 1; //start at sort

	displayString(2, "Press Orange to");
	displayString(3, "toggle modes.");

	while(nNxtButtonPressed== -1){}
	if (nNxtButtonPressed == 3)
	{
		x = -x;
		ClearMessage();
		sendMessage(x);
	}
	else	// != 3
	{
		ClearMessage();
		sendMessage(0);
		return;
	}
	//while(nNxtButtonPressed != 1){}
}



////////////////////////////////Main Task///////////////////////////////////
task main()//to send preferences to the robot
{
	int first_int=0, third_int=-1; bool random1 = true;
	//BT calibration user to slave(robot)
	BTMasterCalibrate(0);
	wait1Msec(150);

	//I think this is needed
	/*ClearMessage();
	while(!bQueuedMsgAvailable()){}
	eraseDisplay();
	displayString(2, "%d", message);
	wait1Msec(2000);
	//this might mess up
	while(bQueuedMsgAvailable()){}
*/

	BTMasterCalibrate(1);

	//Get colours from users
	first_int = inputcolours();

	eraseDisplay();
	displayString(4, "%d" , first_int);
	wait1Msec(2000);

	if(first_int !=0)
	{
		random1 = inputstyle();
	}

	//if the user wants a specified order
	if(!random1)
	{
		third_int = inputorder();
		eraseDisplay();
		displayString(4, "%d" , third_int);

	}

	//if user wants a random assortment
	else
	{
		//11111 wil activate the random shooting algorithm
		third_int=11111;
	}

	//send the preferences to slave
		sendMessageWithParm(first_int, third_int, 0);

		//Display the colour choice
		//wait for user prompt to start robot

		DisplayChoice();

		BTMasterStartOperation();

		//Toggle function

		eraseDisplay();
		displayString(2,"it worked !");
		wait1Msec(2000);

		/*do
		{
			int i=0;
			displayString(7,"you toggled""%d""times",i);
			wait1Msec(200);
			BTMasterToggle();
			wait1Msec(200);
			i++;
			ClearMessage();
		}while(nNxtButtonPressed ==-1);
		//while(!bQueuedMsgAvailable());*/

}
