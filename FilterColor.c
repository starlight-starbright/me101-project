int color_choices[5] = {0, 1, 3, 2, 3};

bool FilterColor (int color);

task main()
{

}

bool FilterColor (int color)
{
	if (color_choices[color - 1] != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
