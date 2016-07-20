#include "NXTServo-lib-UW.c"

#define SENSOR_SERVO S1
#define SENSOR_COLOR S2
#define SENSOR_TOUCH S3
#define SERVO_UPPER 1
#define SERVO_UPPER_POS1 65
#define SERVO_UPPER_POS2 -70
#define SERVO_LOWER 2
#define SERVO_LOWER_POS1 35
#define SERVO_LOWER_POS2 -60
#define MOTOR_SORTER 0
#define MOTOR_STORAGE 1
#define MOTOR_LAUNCHER 2
#define SPEED_MAX_LAUNCHER 90
#define SPEED_MAX_SORTER 5
#define CHAMBER_CAPACITY 12
#define REJECT -1
#define BLANK 0
#define PURPLE 1
#define ORANGE 2
#define GREEN 3
#define YELLOW 4
#define RED 5

//Global arrays
int storage_colors[6] = {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK};
int storage_counts[6] = {0, 0, 0, 0, 0, 0};
bool color_allow[5] = {false, false, false, false, false};
int color_priorities[5] = {0, 0, 0, 0, 0};

//Function Prototypes
void RotateServo (int servo_num, int position1, int position2, int delay_ms);
void RotateMotor (int motor_num, int degrees, int speed_max);
void RotateMotorSimple (int motor_num, int degrees, int speed);
void BTSlaveCalibrate (int motor_num);
void BTSlaveGetPreferences (int & code_color, int & code_order);
void DecodePreferences (int preference_color, int preference_order);
void BTSlaveStartOperation ();
int ScanColor();
int StoreColor (int color);
int TurnToColor (int chamber_requested, int chamber_current, int side);


task main()
{
	//configure servo controller port
	SensorType[SENSOR_SERVO] = sensorI2CCustom9V;

	//configure sensors
	//SensorType[SENSOR_COLOR] = sensor
	//SensorType[

	int preference_code_color = 0;
	int preference_code_order = 0;
	int color = 0;
	int chamber_new = 0;
	int chamber_old = 0;

	//calibrate
	//BTSlaveCalibrate (MOTOR_SORTER);
	//BTSlaveCalibrate (MOTOR_STORAGE);
	//get settings
	//BTSlaveGetPreferences (preference_code_color, preference_code_order);
	//DecodePreferences (preference_code_color, preference_code_order);

	//receive start signal
	//BTSlaveStartOperation ();

	//sort
	RotateServo (SERVO_LOWER, -40, -80, 1000);

	while(true)
	{
		RotateServo (SERVO_UPPER, SERVO_UPPER_POS1, SERVO_LOWER_POS2, 1000);
		RotateMotorSimple (MOTOR_SORTER, 90, SPEED_MAX_SORTER);
		//Parse color
		color = ScanColor();
		eraseDisplay();
		displayString(0, "%d", color);
		if (color != 0 && color != -1)
		{
			chamber_new = StoreColor(color);
			TurnToColor(chamber_new, chamber_old, 1);
			RotateMotorSimple (MOTOR_SORTER, 90, SPEED_MAX_SORTER);
		}
	}
	//fire

}

void RotateServo (int servo_num, int position1, int position2, int delay_ms)
{
	setServoPosition(SENSOR_SERVO, servo_num, position1);
	wait1Msec(delay_ms);
	setServoPosition(SENSOR_SERVO, servo_num, position2);
	wait1Msec(delay_ms);
}

void RotateMotor (int motor_num, int degrees, int speed_max)
{
	const int kP = 150; //actual constants are multiplied by 100
	const int kI = 0;
	const int kD = 0;
	int integral;
	int error_current;
	int error_last;
	int derivative;
	int power;

	nMotorEncoder[motor_num] = 0;

	for (int i = 0; i < 100; i++)
	{
		error_current = degrees - nMotorEncoder[motor_num];
		integral += error_current;
		derivative = error_current - error_last;

		power = kP * error_current + kI * integral + kD * derivative;
		power /= 100;

		if (abs(power) > speed_max)
		{
			power = power / abs(power) * speed_max;
		}

		motor[motor_num] = power;
		error_last = error_current;
		wait1Msec(10);
	}
	motor[motor_num] = 0;
}

void RotateMotorSimple (int motor_num, int degrees, int speed)
{
	nMotorEncoder[motor_num] = 0;
	motor[motor_num] = degrees/abs(degrees) * speed;
	while (abs(nMotorEncoder[motor_num]) < abs(degrees) - 1){}
	motor[motor_num] = 0;
}

int ScanColor()
{
	int color1 = 0;
	int color2 = 0;
	int color3 = 0;
	int red = 0;
	int blue = 0;
	int green = 0;

	SensorType[S2] = sensorColorNxtFULL;

	do
	{
			// recives saturation values of Red, Blue, and Green
			SensorType[S2] = sensorColorNxtRED;
			wait10Msec(25);
			red = SensorRaw[S2];

			SensorType[S2] = sensorColorNxtBLUE;
			wait10Msec(25);
			blue = SensorRaw[S2];

			SensorType[S2] = sensorColorNxtGREEN;
			wait10Msec(25);
			green = SensorRaw[S2];

			color3 = color2;
			color2 = color1;

			// Checks RGB values to be within tested values
			if (red <= 290 && red >= 140 &&
					blue <= 160 && blue >=60 &&
					green <= 165 && green >= 55)
			{
				color1 = 1;// Purple
			}
			else if (red <= 465 && red >= 400 &&
							 blue <= 170 && blue >= 50 &&
							 green <= 215 && green >= 80)
			{
				color1 = 2;// Orange
			}
			else if (red <= 310 && red >= 150 &&
							 blue <= 160 && blue >= 65 &&
							 green <= 260 && green >= 175)
			{
				color1 = 3;// Green
			}
			else if (red <= 490 && red >= 406 &&
							 blue <= 190 && blue >= 65 &&
						   green <= 320 && green >= 220)
			{
				color1 = 4;//Yellow
			}
			else if (red <= 395  && red >= 320 &&
							 blue <= 130 &&	blue >= 50 &&
							 green <= 130 && green >= 50)
			{
				color1 = 5; //Red
			}
			else if (red >= 440 && blue >= 340 && green >= 350)
			{
				color1 = 0; //White, Empty
			}
			else
			{
				color1 = -1;
			}
	} while (color1 != color2 && color1 != color3);

	return color1;
}

//receives two 5 digit int values of binary digits and decodes into two arrays
void DecodePreferences (int preference_color, int preference_order)
{
	color_allow[0] = (bool)(preference_color / 10000);
	color_allow[1] = (bool)((preference_color % 10000) / 1000);
	color_allow[2] = (bool)((preference_color % 1000) / 100);
	color_allow[3] = (bool)((preference_color % 100) / 10);
	color_allow[4] = (bool)(preference_color % 10);

	if (preference_order == -1)
	{
		for(int i = 0; i < 5; i++)
		{
			color_priorities[i] = 1;
		}
	}
	else
	{
		color_priorities[0] = preference_order / 10000;
		color_priorities[1] = (preference_order % 10000) / 1000;
		color_priorities[2] = (preference_order % 1000) / 100;
		color_priorities[3] = (preference_order % 100) / 10;
		color_priorities[4] = preference_order % 10;
	}
}

void BTSlaveStartOperation ()
{
	while(!bQueuedMsgAvailable()){}
	while(bQueuedMsgAvailable())
	{
		return;
	}
}

int ScanColor()
{
	int color1 = 0;
	int color2 = 0;
	int color3 = 0;
	int red = 0;
	int blue = 0;
	int green = 0;

	SensorType[S2] = sensorColorNxtFULL;

	do
	{
			// recives saturation values of Red, Blue, and Green
			SensorType[S2] = sensorColorNxtRED;
			wait10Msec(25);
			red = SensorRaw[S2];

			SensorType[S2] = sensorColorNxtBLUE;
			wait10Msec(25);
			blue = SensorRaw[S2];

			SensorType[S2] = sensorColorNxtGREEN;
			wait10Msec(25);
			green = SensorRaw[S2];

			color3 = color2;
			color2 = color1;

			// Checks RGB values to be within tested values
			if (red <= 290 && red >= 150 &&
					blue <= 155 && blue >=60 &&
					green <= 165 && green >= 60)
			{
				color1 = 1;// Purple
			}
			else if (red <= 465 && red >= 397 &&
							 blue <= 170 && blue >= 50 &&
							 green <= 215 && green >= 80)
			{
				color1 = 2;// Orange
			}
			else if (red <= 310 && red >= 150 &&
							 blue <= 160 && blue >= 65 &&
							 green <= 260 && green >= 175)
			{
				color1 = 3;// Green
			}
			else if (red <= 490 && red >= 406 &&
							 blue <= 190 && blue >= 65 &&
						   green <= 320 && green >= 220)
			{
				color1 = 4;//Yellow
			}
			else if (red <= 396  && red >= 320 &&
							 blue <= 130 &&	blue >= 50 &&
							 green <= 130 && green >= 50)
			{
				color1 = 5; //Red
			}
			else if (red >= 440 && blue >= 340 && green >= 350)
			{
				color1 = 0; //White, Empty
			}
			else
			{
				color1 = -1;
			}
	} while (color1 != color2 && color1 != color3);

	return color1;
}

int StoreColor (int color)
{
	for (int i = 0; i < 6; i++)
	{
		if (storage_colors[i] == color && storage_counts[i] < 12)
		{
			storage_colors[i]++;
			return i;
		}
		else if (storage_counts[i] == 0)
		{
			storage_colors[i] = color;
			storage_colors[i]++;
			return i;
		}
	}
	return -1;
}

//Turns to a requested chamber and returns the new chamber number
int TurnToColor (int chamber_requested, int chamber_current, int side)
{
	int delta = 0;
	int degrees = 0;

	if (side == 1)
	{
		chamber_requested = (chamber_requested + 3) % 6;
	}

	if (chamber_requested != chamber_current)
	{
		delta = abs(chamber_requested - chamber_current);
		if (delta > 3)
		{
			delta = 3 - delta;
		}
		degrees = (60 * delta);
		RotateMotorSimple (MOTOR_STORAGE, degrees, 5);
	}
	return chamber_requested;
}
