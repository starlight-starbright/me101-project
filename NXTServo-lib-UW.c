/**********************************************************************
  Filename: NXTServo-lib-UW.c
  Date: Jan 12, 2015
  File Version: 1.1

  A wrapper for the drivers provided by Mindsensor for use with the
  NXTServo interface module.

  History
  Ver  Date       Comment
  1.1  Jan 12/14  restrict to valid servo numbers 1-7; added SetGripperPos
                  to limit gripper servo position;  limit servo position
                  to 0 to 180; added constants, valid setting function,
                  parameter check function;
                  generalized to use any NXT sensor port;
  1.0  Dec 18/14  original release

**********************************************************************/
#include "NXTServo-lib.c"

// available servo numbers
const int MIN_SERVO_NUM = 1;
const int MAX_SERVO_NUM = 7;  // assumes SV8 used for power

// continous servo
const int MAX_SERVO_SPEED = 100;  // max at +/- 100
const int SERVO_NEUTRAL = 1550;  // 1550 us is neutral position (off)
const int REV_DEADBAND = -25;  // remove some of the deadband that is common to each motor
const int FWD_DEADBAND = 25;

// standard servo position limits
const int MIN_SERVO_ANGLE = 0;  // numbers are from datasheets, verified w/ servo
const int MAX_SERVO_ANGLE = 180;
const int SERVO_OFFSET = 90;  // make zero to be the neutral position
const int SERVO_ZERO = 600;  // 600 us is 0 angle for servo
const int MIN_GRIP_ANGLE = 50;  // numbers are based on tests with gripper
const int MAX_GRIP_ANGLE = 125;

// default I2C address (factory setting)
const int I2C_ADDR = 0xb0;

// control register
const ubyte CONTROL_REG_ADDR = 0x41;

void setValidSetting(int & setting, int minSetting, int maxSetting)
{
    if (setting > maxSetting)
        setting = maxSetting;
    else if (setting < minSetting)
        setting = minSetting;
}

bool paramIsValid(tSensors nxtPort, int servoNumber)
{
    return (nxtPort == S1 || nxtPort == S2 || nxtPort == S3 || nxtPort == S4)
          && servoNumber >= MIN_SERVO_NUM && servoNumber <= MAX_SERVO_NUM;
}

void setServoSpeed(tSensors nxtPort, int servo_number, int speed_setting, int neg_offset = 0, int pos_offset = 0)
{
    if (paramIsValid(nxtPort,servo_number))
    {
        setValidSetting(speed_setting, -MAX_SERVO_SPEED, MAX_SERVO_SPEED);

        if (speed_setting == 0)
            NXTServo_SetPosition(nxtPort, I2C_ADDR, servo_number, SERVO_NEUTRAL);
        else if (speed_setting > 0)
            NXTServo_SetPosition(nxtPort, I2C_ADDR, servo_number, SERVO_NEUTRAL + FWD_DEADBAND + speed_setting + pos_offset);
        else
            NXTServo_SetPosition(nxtPort, I2C_ADDR, servo_number, SERVO_NEUTRAL + REV_DEADBAND + speed_setting + neg_offset);
    }
}

void setServoPosition(tSensors nxtPort, int servo_number, int position)
{
    if (paramIsValid(nxtPort,servo_number))
    {
        // zero is neutral position
        position = position + SERVO_OFFSET;

        setValidSetting(position, MIN_SERVO_ANGLE, MAX_SERVO_ANGLE);

        //now convert to servo signals and send.  The servo takes a signal from 600uS (-90 degrees) to 2400uS (90 degrees)
        NXTServo_SetPosition(nxtPort, I2C_ADDR, servo_number, SERVO_ZERO + (position)*10);
    }
}

/*
  Set gripper position based on angle - normed to allow 0 to 75 as valid values
*/
void setGripperPosition(tSensors nxtPort, int servo_number, int position)
{
    if (paramIsValid(nxtPort,servo_number))
    {
        // zero commanded position is minimum angle
        position = position + MIN_GRIP_ANGLE;

        setValidSetting(position, MIN_GRIP_ANGLE, MAX_GRIP_ANGLE);

        //now convert to servo signals and send.  The servo takes a signal from 600uS (-90 degrees) to 2400uS (90 degrees)
        NXTServo_SetPosition(nxtPort, I2C_ADDR, servo_number, SERVO_ZERO + position*10);
    }
}

void resetGripper(tSensors nxtPort, int servoNumber)
{
    setGripperPosition(nxtPort, servoNumber, 90 - MIN_GRIP_ANGLE);
}

/*
  The two functions below have not been tested.  They should reset the interface to its
  factory settings.  They can be tested after the first problems with an interface,
  otherwise at the moment it is best to leave things as is.
// I2C message send is copied from the code in NXTServo-lib.c
void NXTServo_SendCommand(tSensors port, ubyte cmd)
{
  ubyte sc8Msg[5];
  const int kMsgSize          = 0;
  const int kSc8Address       = 1;
  const int kSc8Register      = 2;
  const int kData             = 3;

  // Build the I2C message
  sc8Msg[kMsgSize]            = 3;
  sc8Msg[kSc8Address]         = I2C_ADDR;
  sc8Msg[kSc8Register]        = CONTROL_REG_ADDR;
  sc8Msg[kData]               = cmd;

  while (nI2CStatus[port] == STAT_COMM_PENDING);
  {
   // Wait for I2C bus to be ready
  }
  // when the I2C bus is ready, send the message you built
  sendI2CMsg(port, sc8Msg[0], 0);
}

void resetServoInterface(tSensors port)
{
    NXTServo_SendCommand(port, (ubyte)'S');
}
*/
/**********************************************************************
  Copyright(c) 2014-2015 D. Lau, M. Stachowsky, and C.C.W. Hulls, P.Eng.
  Students, staff, and faculty members at the University of Waterloo
  are granted a non-exclusive right to copy, modify, or use this
  software for non-commercial teaching, learning, and research purposes
  provided the author(s) are acknowledged except for as noted below.

  Winter 2015 ME 101 students can copy, modify, or use this software
  in their Winter 2015 ME 101 course assignments without acknowledging
  the source.
**********************************************************************/
