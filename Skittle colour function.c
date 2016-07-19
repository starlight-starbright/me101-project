int skittle_colour()
{
	int colour1 = 0;
	int color2 = 0;
	int red = 0;
	int green = 0;
	int blue = 0;

	do
	{
			// receives RGB values
			SensorType[S2] = sensorColorNxtRED;
			wait10Msec(25);
			red = SensorRaw[S2];

			SensorType[S2] = sensorColorNxtBLUE;
			wait10Msec(25);
			blue = SensorRaw[S2];

			SensorType[S2] = sensorColorNxtGREEN;
			wait10Msec(25);
			gre = SensorRaw[S2];

			pre_colour = colour;

			// Checks RGB values to be within pre-tested values
			if (red <= 290 && red >= 165 && blue <=150 &&
				blue >=60 && gre <= 145 && gre >= 85)
			{
				colour = 1;// Purple
			}
			else if (red <= 490 && red >= 405 && blue <= 140 &&
				blue >= 55 && gre <= 210 && gre >= 80)
			{
				colour = 2;// Orange
			}
			else if (red <= 150 && red >= 30 && blue <= 225 &&
				blue >= 90 && gre <= 270 && gre >= 200)
			{
				colour = 3;// Green
			}
			else if (red <= 500 && red >= 430 && blue <= 220 &&
				blue >= 70 && gre <= 320 && gre >= 230)
			{
				colour = 4;// yellow
			}
			else if (red <= 410 && red >= 360 && blue <= 110 &&
				blue >=60 && gre <= 110 && gre >= 70)
			{
				colour = 5; // Red
			}
			else if (red >= 460 && blue >= 380 && gre >= 380)
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
			displayString(0, "Red = %d", colour_value);
		}

	}while(nNxtButtonPressed != 0);
}
