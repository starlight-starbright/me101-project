int storage_colors[6] = {0, 0, 0, 0, 0, 0};
int storage_counts[6] = {0, 0, 0, 0, 0, 0};

bool StoreCandy (int color);

task main()
{

}

bool StoreCandy (int color)
{
	for (int i = 0; i < 6; i++)
	{
		if (storage_colors[i] == 0 || storage_counts[i] == 0)
		{
			storage_colors[i] = color;
			storage_colors[i]++;
		}
	}
}
