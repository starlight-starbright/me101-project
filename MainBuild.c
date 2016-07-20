#include "NXTServo-lib-UW.c"

#define SENSOR_SERVO S1
#define SENSOR_COLOR S2
#define SENSOR_TOUCH S3
#define SERVO_UPPER 1
#define SERVO_UPPER_POS1 60
#define SERVO_UPPER_POS2 -70
#define SERVO_LOWER 2
#define SERVO_LOWER_POS1 35
#define SERVO_LOWER_POS2 -60
#define MOTOR_SORTER 0
#define MOTOR_STORAGE 1
#define MOTOR_LAUNCHER 2
#define SPEED_MAX_SORTER 3
#define SPEED_MAX_STORAGE 5
#define SPEED_MAX_LAUNCHER 85
#define CHAMBER_CAPACITY 12
#define REJECT -1
#define BLANK 0
#define PURPLE 1
#define ORANGE 2
#define GREEN 3
#define YELLOW 4
#define RED 5

//Global arrays
int color_choices[5] = {0, 0, 0, 0, 0};
int storage_colors[6] = {BLANK, BLANK, BLANK, BLANK, BLANK, BLANK};
int storage_counts[6] = {0, 0, 0, 0, 0, 0};
//int color_counts[5] = {0, 0, 0, 0, 0};
//int colors_stored[5] = {0, 0, 0, 0, 0};
//int colors_fired[5] = {0, 0, 0, 0, 0};

//Function prototypes
void RotateServo (int servo_num, int position1, int position2, int delay_ms);
void RotateMotor (int motor_num, int degrees, int speed_max);
void RotateMotorSimple (int motor_num, int degrees, int speed);
void BTSlaveCalibrate (int motor_num);
int BTSlaveGetPreferences ();
void DecodePreferences (int code_choices);
void BTSlaveStartOperation ();
void BTSlaveToggleMode ();
int TurnToColor (int chamber_requested, int chamber_current, int side);
int ScanColor();
bool FilterColor (int color);
int StoreColor (int color);
int PickColor();
int SendColor (int color);
void SpoolLaunchMotor (int motor_num, bool spool_up, int power_max);

task main()
{
	//configure servo controller port
	SensorType[SENSOR_SERVO] = sensorI2CCustom9V;

	//configure sensors
	//SensorType[SENSOR_COLOR] = sensor
	//SensorType[

	int preference_code_color = 0;
	int preference_code_order = 0;
	int mode = 0; // 0 - sort, 1 - fire, -1 = quit
	int color = 0;

	//calibrate
	BTSlaveCalibrate (MOTOR_SORTER);
	BTSlaveCalibrate (MOTOR_STORAGE);
	eraseDisplay();

	//get settings and decode
	DecodePreferences (BTSlaveGetPreferences());

	//receive start signal
	BTSlaveStartOperation ();

	//main operation
	BTSlaveToggleMode ();
	displayString(7, "asdf");

	//send termination report
	//BTSlaveSendReport ();
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
	while (abs(nMotorEncoder[motor_num]) < abs(degrees)){}
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

int BTSlaveGetPreferences ()
{
	int code = 0;
	while(!bQueuedMsgAvailable()){}
	while(bQueuedMsgAvailable())
	{
		ClearMessage();
		code = messageParm[0];
	}
	return code;
}

//receives a 5 digit int composed of binary digits and decodes into an array
void DecodePreferences (int code_choices)
{
	color_choices[0] = code_choices / 10000;
	color_choices[1] = code_choices % 10000 / 1000;
	color_choices[2] = code_choices % 1000 / 100;
	color_choices[3] = code_choices % 100 / 10;
	color_choices[4] = code_choices % 10;
}

void BTSlaveStartOperation ()
{
	displayString(0,"waiting");
	while(!bQueuedMsgAvailable()){}
		return;
}

void BTSlaveToggleMode ()
{
	int mode = 1;
	int color = 0;
	int chamber_requested = 0;
	int chamber_current = 0;

	while (mode != 0)
	{
		if (mode == 1) //sort
		{
			//sort once
			eraseDisplay();
			displayString (0, "SORT");
			RotateServo (SERVO_UPPER, SERVO_UPPER_POS1, SERVO_UPPER_POS2, 1000);
			RotateMotorSimple (MOTOR_SORTER, 90, SPEED_MAX_SORTER);
			color = ScanColor();
			eraseDisplay();
			displayString(0, "%d", color);
			if (FilterColor(color))
			{
				chamber_current = TurnToColor(StoreColor(color), chamber_current, 1);
				displayString(1, "%d", StoreColor(color));
				displayString(1, "%d", chamber_current);
				RotateMotorSimple (MOTOR_SORTER, 90, SPEED_MAX_SORTER);
			}
			else
			{
				RotateMotorSimple (MOTOR_SORTER, -180, SPEED_MAX_SORTER);
				wait1Msec (250);
				RotateMotorSimple (MOTOR_SORTER, 90, SPEED_MAX_SORTER);
			}
		}
		else if (mode == -1) // fire
		{
			//fire once
			eraseDisplay();
			displayString (0, "FIRE");
			color = PickColor();
			chamber_current = TurnToColor(SendColor(color), chamber_current, 0);
			SpoolLaunchMotor (MOTOR_LAUNCHER, true, SPEED_MAX_LAUNCHER);
			wait1Msec(2000);
			RotateServo (SERVO_LOWER, SERVO_LOWER_POS1, SERVO_LOWER_POS2, 1000);
			wait1Msec(2000);
			SpoolLaunchMotor (MOTOR_LAUNCHER, false, SPEED_MAX_LAUNCHER);
			wait1Msec(2000);
		}
		if(bQueuedMsgAvailable())
		{
			ClearMessage();
			mode = messageParm[0];
			displayString(5, "%d", mode);
		}
		wait1Msec (50);
	}
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
		RotateMotorSimple (MOTOR_STORAGE, degrees, SPEED_MAX_STORAGE);
	}
	return chamber_requested;
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

bool FilterColor (int color)
{
	if (color == REJECT || color == BLANK)
	{
		return false;
	}
	else if (color_choices[color - 1] == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
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

int PickColor()
{
	int priority1[5]={0,0,0,0,0};
	int priority2[5]={0,0,0,0,0};
	int priority3[5]={0,0,0,0,0};
	int storage_sum = 0;
	int count1 = 0;
	int count2 = 0;
	int count3 = 0;
	int randomx=0;
	bool match = false;

	//check for all empty
	for (int i = 0; i < 6; i ++)
	{
		storage_sum = storage_counts[i];
	}
	if (storage_sum == 0)
	{
		return -1;
	}

	//check for priority 1
	for (int i = 0; i < 5; i++)
	{
		match = false;
		if (color_choices[i] == 1)
		{
			for(int j = 0; j < 6; j++)
			{
				if (storage_colors[j] == i + 1 && storage_counts[j]>0)
				{
					priority1[i]=1;
					if (match == false)
					{
						count1++;
						match = true;
					}
				}
			}
		}
	}

	//check for priority 2
	for (int i = 0; i < 5; i++)
	{
		match = false;
		if (color_choices[i] == 2)
		{
			for(int j = 0; j < 6; j++)
			{
				if (storage_colors[j] == i + 1 && storage_counts[j]>0)
				{
					priority2[i] = 1;
					if (match == false)
					{
						count2++;
						match = true;
					}
				}
			}
		}
	}

	//check for priority 3
	for (int i = 0; i < 5; i++)
	{
		match = false;
		if (color_choices[i] == 3)
		{
			for(int j = 0; j < 6; j++)
			{
				if (storage_colors[j] == i + 1 && storage_counts[j]>0)
				{
					priority3[i] = 1;
					if (match == false)
					{
						count3++;
						match = true;
					}
				}
			}
		}
	}

	if (count1 != 0)
	{
		do
		{
			randomx = random(4);
			if (priority1[randomx] == 1)
			{
				randomx ++;
				return randomx;
			}
		} while(true);
	}

	if (count2 != 0)
	{
		do
		{
			randomx = random(4);
			if (priority2[randomx] == 1)
			{
				randomx++;
				return randomx;
			}
		} while(true);
	}

	if (count3 != 0)
	{
		do
		{
			randomx = random(4);
			if (priority3[randomx] == 1)
			{
				randomx ++;
				return randomx;
			}
		} while(true);
	}
	return -1;
}

int SendColor (int color)
{
	for(int i = 0; i < 5; i++)
	{
		if (storage_colors[i] == color && storage_counts[i] > 0)
		{
			storage_counts[i]--;
			if (storage_counts[i] == 0)
			{
				storage_colors[i] = 0;
			}
			return i;
		}
	}
	return -1;
}

void SpoolLaunchMotor (int motor_num, bool spool_up, int power_max)
{
	if (spool_up)
	{
		time1[1] = 0;
		while(time1[1] <= 1000)
		{
			motor[motor_num] = (int)(1.0 * power_max * time1[1] / 1000.0);
			wait1Msec(25);
		}
	}
	else
	{
		time1[1] = 0;
		while(time1[1] <= 1000)
		{
			motor[motor_num] = (int)(1.0 * power_max * (-time1[1] + 1000));
			wait1Msec(25);
		}
	}
}
