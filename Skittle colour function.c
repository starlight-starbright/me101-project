int skittle_colour()
{
	int colour = 6, pre_colour = 6, red = 0, blue = 0, green = 0;
	SensorType[S2] = sensorColorNxtFULL;

	do
	{
			// recives saturation values of Red, Blue, and Green
			SensorType[S2] = sensorColorNxtRED;
			wait10Msec(25);
			red = SensorRaw[S2];

			SensorType[S2] = sensorColorNxtBLUE;
			wait10Msec(25);
			blue = SensorRaw[S2];

			SensorType[S2] = sensorColorNxtGREEN;
			wait10Msec(25);
			green = SensorRaw[S2];

			pre_colour = colour;

			// Checks RBG values to be with in tested values
			if (red <= 290 && red >= 150 && blue <= 155 &&
				blue >=60 && green <= 165 && green >= 60)
			{
				colour = 1;// Purple

			}
			else if (red <= 465 && red >= 397 && blue <= 170 &&
				blue >= 50 && green <= 215 && green >= 80)
			{
				colour = 2;// Orange
			}
			else if (red <= 310 && red >= 150 && blue <= 160 &&
				blue >= 65 && green <= 260 && green >= 175)
			{
				colour = 3;// Green
			}
			else if (red <= 490 && red >= 406 && blue <= 190 &&
				blue >= 65 && green <= 320 && green >= 220)
			{
				colour = 4;// yellow
			}
			else if (red <= 396  && red >= 320 && blue <= 130 &&
				blue >= 50 && green <= 130 && green >= 50)
			{
				colour = 5; // Red
			}
			else if (red >= 440 && blue >= 340 && green >= 350)
			{
				colour = 0; //White, Empty
			}
			else
			{
				colour = -1;
			}
	}
		while (colour != pre_colour);

	return colour;
}

task main()
{
	SensorType[S2] = sensorColorNxtFULL;
	int colour_value = 0;

	do
	{
		while(nNxtButtonPressed == 2)
		{
			motor[motorA] = 5;
		}

		while(nNxtButtonPressed == 1)
		{
			motor[motorA] = -5;
		}
		motor[motorA] = 0;

		if (nNxtButtonPressed == 3)
		{
			eraseDisplay();
			colour_value = skittle_colour();
			displayString(0, "Colour %d", colour_value);
		}

	}while(nNxtButtonPressed != 0);
}
