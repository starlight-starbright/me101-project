int storage_colors[6] = {2, 3, 5, 4, 0, 0};
int storage_counts[6] = {1, 2, 4, 12, 0, 0};

int StoreColor (int color);

task main()
{
	int x = StoreColor(4);
	displayString(0, "%d", x);
	wait1Msec(1000);
}

int StoreColor (int color)
{
	for (int i = 0; i < 6; i++)
	{
		if (storage_colors[i] == color && storage_counts[i] < 12)
		{
			storage_counts[i]++;
			return i;
		}
		else if (storage_counts[i] == 0)
		{
			storage_colors[i] = color;
			storage_counts[i]++;
			return i;
		}
	}
	return -1;
}
