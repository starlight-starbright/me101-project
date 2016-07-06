task arm_emergency_stop();
void rotate(int motor_number, int degrees);
void store_color(int color);
int get_color();

task main()
{
	startTask(arm_emergency_stop);

	int color = 0;
	SensorType[S1] = sensorColorNxtFULL;

	int colorAccept[6] = {1, 0, 0, 0, 1, 0};
	//1 - Black/Purple
	//2 - Blue
	//3 - Green
	//4 - Yellow
	//5 - Red
	//6 - White

	while(true)
	{
		//Rotate to color sensor
		rotate(0, 90);
		wait1Msec(250);

		//Detect color
		color = get_color();
		displayString(0, "Color: %d", color);
		wait1Msec(1000);
		eraseDisplay();

		if(colorAccept[color - 1] == 1)
		{
			store_color(color);
			wait1Msec(250);
			rotate(0, 90);
			wait1Msec(250);

		}
		else //color rejected
		{
			rotate (0, -180);
			wait1Msec(250);
			rotate (0, 90);
			wait1Msec(250);
		}
		EndTimeSlice();
	}
}

task arm_emergency_stop()
{
	while(true)
	{
		if(SensorValue[S2] == 1)
		{
			stopAllTasks();
		}
		EndTimeSlice();  // this prevents the current task from hogging the CPU
	}
}

void rotate(int motor_number, int degrees)
{
	nMotorEncoder[motor_number] = 0;
	motor[motor_number] = (degrees / abs(degrees)) * 5;
	while(abs(nMotorEncoder[motor_number]) < abs(degrees)){}
	motor[motor_number] = 0;
}

void store_color(int color)
{
	rotate(1, color * 60);
}

int get_color() //update later for full RGB if possible
{
	int color = SensorValue[S1];
	return color;
}
