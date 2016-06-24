void rotate(int degrees);

task main()
{
	int color = 0;
	SensorType[S1] = sensorColorNxtFULL;

	int colorAccept[6] = {1, 0, 0, 0, 1, 0};
	/*
	1 - Black/Purple
	2 - Blue
	3 - Green
	4 - Yellow
	5 - Red
	6 - White
	*/



	while(true)
	{
		//Rotate to color sensor
		rotate(90);
		wait1Msec(250);

		//Detect color
		color = SensorValue[S1];
		displayString(0, "%d", color);
		wait1Msec(1000);
		eraseDisplay();

		if(colorAccept[color - 1] == 1)
		{
			rotate(90);
			wait1Msec(250);
		}
		else //color rejected
		{
			rotate (-180);
			wait1Msec(250);
			rotate (90);
			wait1Msec(250);
		}
	}
}

void rotate(int degrees)
{
	//Rotate to color sensor
	nMotorEncoder[0] = 0;
	motor[0] = (degrees / abs(degrees)) * 5;
	while(abs(nMotorEncoder[0]) < abs(degrees)){}
	motor[0] = 0;
}
