int ScanColor();

task main()
{
	while(true)
	{
		int x = ScanColor ();
		eraseDisplay();
		displayString (0, "%d", x);
		wait1Msec(250);
	}
}

int ScanColor()
{
	int color1 = 0;
	int color2 = 0;
	int color3 = 0;
	int red = 0;
	int blue = 0;
	int green = 0;

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

			color3 = color2;
			color2 = color1;

			// Checks RGB values to be within tested values
			if (red <= 290 && red >= 140 &&
					blue <= 160 && blue >=60 &&
					green <= 165 && green >= 55)
			{
				color1 = 1;// Purple
			}
			else if (red <= 465 && red >= 400 &&
							 blue <= 170 && blue >= 50 &&
							 green <= 215 && green >= 80)
			{
				color1 = 2;// Orange
			}
			else if (red <= 310 && red >= 150 &&
							 blue <= 160 && blue >= 65 &&
							 green <= 260 && green >= 175)
			{
				color1 = 3;// Green
			}
			else if (red <= 490 && red >= 406 &&
							 blue <= 190 && blue >= 65 &&
						   green <= 320 && green >= 220)
			{
				color1 = 4;//Yellow
			}
			else if (red <= 395  && red >= 320 &&
							 blue <= 130 &&	blue >= 50 &&
							 green <= 130 && green >= 50)
			{
				color1 = 5; //Red
			}
			else if (red >= 440 && blue >= 340 && green >= 350)
			{
				color1 = 0; //White, Empty
			}
			else
			{
				color1 = -1;
			}
	} while (color1 != color2 && color1 != color3);

	return color1;
}
