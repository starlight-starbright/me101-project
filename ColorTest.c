task main()
{
	int color = 0;
	SensorType[S3] = sensorColorNxtFULL;

	while (nNxtButtonPressed != 3)
	{
		eraseDisplay();
		color = SensorValue[S3];
		displayString (0, "Color Value: %i", color);
		wait10Msec(100);
	}
}
