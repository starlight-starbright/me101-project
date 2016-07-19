int storage_colors[6] = {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK};
int storage_counts[6] = {0, 0, 0, 0, 0, 0};
bool allow_color[5] = {false, false, false, false, false};
int color_priorities[5] = {0, 0, 0, 0, 0};

void SendCandy (int color);

task main()
{

}

void SendCandy (int color)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (color_priorities[j] == i);
	}
}
