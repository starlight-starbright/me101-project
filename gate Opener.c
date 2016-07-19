void openGate ()
{
	motor[motorC] =  60;
	while(nMotorEncoder[motorC] < (100)) {}

	motor[motorC] = -60;
	while(nMotorEncoder[motorC] > 0) {}

	motor[motorC] = 0;
}

task main()
{
	  nMotorEncoder[motorC] = 0;

	  while(true)
	  {
	  	openGate();
	  	wait1Msec(2000);
	  }

}
