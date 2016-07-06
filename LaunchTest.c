task main()
{
	while(true)
	{
		while (nNxtButtonPressed != 3){}
		motor[0] = -90;
		while (nNxtButtonPressed == 3){}
		motor[0] = 0;
	}
}
