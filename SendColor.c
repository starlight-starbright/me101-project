int storage_colors[6] = {0, 0, 0, 0, 0, 0};
int storage_counts[6] = {0, 0, 0, 0, 0, 0};

int SendColor (int color);

task main()
{
	int x = SendColor (3);
}

int SendColor (int color)
{
	for(int i = 0; i < 5; i++)
	{
		if (storage_colors[i] == color && storage_counts[i] > 0)
		{
			storage_counts[i]--;
			if (storage_counts[i] == 0)
			{
				storage_colors[i] = 0;
			}
			return i;
		}
	}
	return -1;
}
