
int colourchoice[5]={0,0,0,0,0};

int inputcolours()
{
	int decision[5] = {0, 0, 0, 0, 0};
	int final = 0;
	string colours[5]={"Purple", "Orange", "Green", "Yellow", "Red"};

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

bool inputstyle()
{
	int buttonval=0;
			eraseDisplay();
			displayString(0, "Would you like ");
			displayString(1,"the color below");
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

int inputorder()
{
	int decision[5] = {0, 0, 0, 0, 0};
	int final = 0;
	string colours[5]={"Purple", "Orange", "Green", "Yellow", "Red"};

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

task main()//to send preferences to the robot
{
	int first_int=0, third_int=-1; bool random1 = true;
	first_int = inputcolours();
	eraseDisplay();
	displayString(4, "%d" , first_int);
		wait1Msec(2000);
	random1 = inputstyle();
	if(!random1)
	{
		third_int = inputorder();
		eraseDisplay();
		displayString(4, "%d" , third_int);
		while(true){}
	}
	else
	{
		third_int=11111;
	}
		sendMessageWithParm(first_int, third_int, 0);

		eraseDisplay();
		displayString(2,"it worked !");
		wait1Msec(2000);


}
