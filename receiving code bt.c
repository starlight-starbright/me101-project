task main()
{
	int a=0, b=0, c=0;
		while(!bQueuedMsgAvailable()){}
		while(bQueuedMsgAvailable())
		{
			a = messageParm[0];
			b = messageParm[1];
			eraseDisplay();
			ClearMessage();
			displayString(2, "%d,%d", a,b);
			wait1Msec(2000);
			while(true){}
		}

}
