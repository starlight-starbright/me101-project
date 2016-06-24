task main()
{
	while(nNxtButtonPressed != 3)
	{
		nMotorEncoder[0] = 0;
		motor[0] = 10;
		while(nMotorEncoder[0] < 90){}
		motor[0] = 0;
		eraseDisplay();
		displayString(0, "%d", nMotorEncoder[0]);
		wait1Msec(500);
	}
}
