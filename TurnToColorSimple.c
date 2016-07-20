task main()
{

}

void TurnToColorSimple (int chamber_requested, int side)
{
	int degrees = 0;

	if (side == 1)
	{
		chamber_requested = (chamber_requested + 3) % 6;
	}

	if (chamber_requested != 0)
	{
		degrees = -60 * chamber_requested;
		RotateMotorSimple (MOTOR_STORAGE, degrees, SPEED_MAX_STORAGE);
		RotateMotorSimple (MOTOR_SORTER, 90, SPEED_MAX_SORTER);
		degrees = -degrees;
		RotateMotorSimple (MOTOR_STORAGE, degrees, SPEED_MAX_STORAGE);
	}
}
