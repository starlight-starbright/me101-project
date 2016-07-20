task main()
{
	int color = 0;
	SensorType[S1] = sensorColorNxtFULL;

	while (true)
	{
		color = SensorValue[S1];
		displayString (0, "Color Value: %i", color);
		wait1Msec(500);
		eraseDisplay();
	}
}
