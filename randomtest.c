task main()
{
	int x;
	while(true)
	{
		x = random(4);
		eraseDisplay();
		displayString(0, "%d", x);
		wait1Msec (250);
	}
}
