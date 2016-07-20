void BTGetPreferences (int & code_color, int & code_order);

task main()
{

}

void BTGetPreferences (int & code_color, int & code_order)
{
	while(!bQueuedMsgAvailable()){}
	while(bQueuedMsgAvailable())
	{
		code_color = messageParm[0];
		code_order = messageParm[1];
	}
}
