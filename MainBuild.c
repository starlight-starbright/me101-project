#include "NXTServo-lib-UW.c"

#define SENSOR_SERVO S1
#define SENSOR_COLOR S2
#define SENSOR_TOUCH S3
#define SERVO_UPPER 1
#define SERVO_UPPER_POS1 55
#define SERVO_UPPER_POS2 -70
#define SERVO_LOWER 2
#define SERVO_LOWER_POS1 35
#define SERVO_LOWER_POS2 -60
#define MOTOR_SORTER 0
#define MOTOR_STORAGE 1
#define MOTOR_LAUNCHER 2
#define SPEED_MAX_LAUNCHER 90
#define SPEED_MAX_SORTER 15
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
bool allow_color[5] = {false, false, false, false, false};
int color_priorities[5] = {0, 0, 0, 0, 0};

//Function Prototypes
void RotateServo (int servo_num, int position1, int position2, int delay_ms);
void RotateMotor (int motor_num, int degrees, int speed_max);
void BTSlaveCalibrate (int motor_num);
void BTSlaveGetPreferences (int & code_color, int & code_order);
void DecodePreferences (int preference_color, int preference_order);
int ScanColor();
int TurnToColor (int chamber_requested, int chamber_current, int side);


task main()
{
	//configure servo controller port
	SensorType[S1] = sensorI2CCustom9V;

	int preference_code_color = 0;
	int preference_code_order = 0;
	int mode = 0; // 0 - sort, 1 - fire
	int color = 0;

	//calibrate
	BTSlaveCalibrate (MOTOR_SORTER);
	BTSlaveCalibrate (MOTOR_STORAGE);
	//get settings
	BTSlaveGetPreferences (preference_code_color, preference_code_order);
	DecodePreferences (preference_code_color, preference_code_order);
	//sort
	while(mode != -1)
	{
		RotateServo (SERVO_UPPER, SERVO_UPPER_POS1, SERVO_LOWER_POS2, 1000);
		RotateMotor (MOTOR_SORTER, 90, SPEED_MAX_SORTER);
		//Parse color
		color = ScanColor();
		while(nNxtButtonPressed == -1){}
		if (nNxtButtonPressed == 3)
		{

			RotateMotor	(MOTOR_SORTER, 90, SPEED_MAX_SORTER);
			wait1Msec (100);

		}
		else
		{
			RotateMotor (MOTOR_SORTER, -180, SPEED_MAX_SORTER);
			wait1Msec (100);
			RotateMotor (MOTOR_SORTER, 90, SPEED_MAX_SORTER);
			wait1Msec (100);
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

void BTSlaveCalibrate (int motor_num)
{
	bool proceed = true;
	int button = 0;

	while(!bQueuedMsgAvailable()){}
	while(bQueuedMsgAvailable())
	{
		ClearMessage();
		proceed = (bool)(messageParm[0]);
		eraseDisplay();
		displayString(0, "MOTOR %d", motor_num);
		wait1Msec(50);
	}
	if (proceed)
	{
		while(!bQueuedMsgAvailable()){}
		while(true)
		{
			ClearMessage();
			button = messageParm[0];
			eraseDisplay();
			displayString (1, "%d", button);
			if (button == 1)
			{
				motor[motor_num] = 10;
				while (button == 1)
				{
					ClearMessage();
					button = messageParm[0];
					wait1Msec(30);
				}
			}
			else if (button == 2)
			{
				motor[motor_num] = -10;
				while (button == 2)
				{
					ClearMessage();
					button = messageParm[0];
					wait1Msec(30);
				}
			}
			else if (button == 3)
			{
				motor[motor_num] = 0;
				return;
			}
			motor[motor_num] = 0;
			wait1Msec(50);
		}
	}
}

void BTSlaveGetPreferences (int & code_color, int & code_order)
{
	while(!bQueuedMsgAvailable()){}
	while(bQueuedMsgAvailable())
	{
		code_color = messageParm[0];
		code_order = messageParm[1];
	}
}

//receives two 5 digit int values of binary digits and decodes into two arrays
void DecodePreferences (int preference_color, int preference_order)
{
	allow_color[0] = (bool)(preference_color / 10000);
	allow_color[1] = (bool)((preference_color % 10000) / 1000);
	allow_color[2] = (bool)((preference_color % 1000) / 100);
	allow_color[3] = (bool)((preference_color % 100) / 10);
	allow_color[4] = (bool)(preference_color % 10);

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

int ScanColor()
{
	int color1 = 0;
	int color2 = 0;
	int red = 0;
	int green = 0;
	int blue = 0;

	do
	{
			// receives RGB values
			SensorType[S2] = sensorColorNxtRED;
			wait10Msec(25);
			red = SensorRaw[S2];

			SensorType[S2] = sensorColorNxtBLUE;
			wait10Msec(25);
			blue = SensorRaw[S2];

			SensorType[S2] = sensorColorNxtGREEN;
			wait10Msec(25);
			green = SensorRaw[S2];

			color2 = color1;

			// Checks RGB values to be within pre-tested values
			if (red >= 460 && blue >= 380 && green >= 380)
			{
				color1 = 0; //Blank
			}
			else if (red <= 290 && red >= 165 && blue <=150 &&
				blue >=60 && green <= 145 && green >= 85)
			{
				color1 = 1;//Purple
			}
			else if (red <= 490 && red >= 405 && blue <= 140 &&
				blue >= 55 && green <= 210 && green >= 80)
			{
				color1 = 2;//Orange
			}
			else if (red <= 150 && red >= 30 && blue <= 225 &&
				blue >= 90 && green <= 270 && green >= 200)
			{
				color1 = 3;//Green
			}
			else if (red <= 500 && red >= 430 && blue <= 220 &&
				blue >= 70 && green <= 320 && green >= 230)
			{
				color1 = 4;//Yellow
			}
			else if (red <= 410 && red >= 360 && blue <= 110 &&
				blue >=60 && green <= 110 && green >= 70)
			{
				color1 = 5; //Red
			}
			else
			{
				color1 = -1; //Unknown
			}
	} while (color1 != color2);

	return color1;
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
		RotateMotor (1, degrees, 15);
	}
	return chamber_requested;
}
