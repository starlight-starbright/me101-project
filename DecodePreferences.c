bool allow_color[5] = {false, false, false, false, false};
int color_priorities[5] = {0, 0, 0, 0, 0};

void DecodePreferences (int preference_color, int preference_order);

task main()
{

}

//receives two 5 digit int values of binary digits and decodes into two arrays
void DecodePreferences (int preference_color, int preference_order)
{
	allow_color[0] = (bool)(preference_color / 10000);
	allow_color[1] = (bool)((preference_color % 10000) / 1000);
	allow_color[2] = (bool)((preference_color % 1000) / 100);
	allow_color[3] = (bool)((preference_color % 100) / 10);
	allow_color[4] = (bool)(preference_color % 10);

	if (preference_order == -1)
	{
		for(int i = 0; i < 5; i++)
		{
			color_priorities[i] = 1;
		}
	}
	else
	{
		color_priorities[0] = preference_order / 10000;
		color_priorities[1] = (preference_order % 10000) / 1000;
		color_priorities[2] = (preference_order % 1000) / 100;
		color_priorities[3] = (preference_order % 100) / 10;
		color_priorities[4] = preference_order % 10;
	}
}
