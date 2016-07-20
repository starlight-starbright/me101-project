void BTSlaveStartOperation ();

task main()
{

}

void BTSlaveStartOperation ()
{
	while(!bQueuedMsgAvailable()){}
	while(bQueuedMsgAvailable())
	{
		return;
	}
}
