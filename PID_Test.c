#define PID_SENSOR_INDEX    0
#define PID_SENSOR_SCALE    1

#define PID_MOTOR_INDEX     0
#define PID_MOTOR_SCALE     -1

#define PID_DRIVE_MAX       10
#define PID_DRIVE_MIN     (-10)

#define PID_INTEGRAL_LIMIT  50

// These could be constants but leaving
// as variables allows them to be modified in the debugger "live"
float  pid_Kp = 2.0;
float  pid_Ki = 0.04;
float  pid_Kd = 0.0;

static int   pidRunning = 1;
static float pidRequestedValue;

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*  pid control task                                                           */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

task pidController()
{
    float  pidSensorCurrentValue;

    float  pidError;
    float  pidLastError;
    float  pidIntegral;
    float  pidDerivative;
    float  pidDrive;

    // If we are using an encoder then clear it
   	nMotorEncoder[ PID_SENSOR_INDEX ] = 0;

   	// Init the variables - thanks Glenn :)
		pidLastError  = 0;
 		pidIntegral   = 0;

    while( true )
        {
        	// Is PID control active ?

    			if( pidRunning )
            {
            // Read the sensor value and scale
            pidSensorCurrentValue = nMotorEncoder[ PID_SENSOR_INDEX ] * PID_SENSOR_SCALE;

            // calculate error
            pidError = pidSensorCurrentValue - pidRequestedValue;

            // integral - if Ki is not 0
            if( pid_Ki != 0 )
                {
                // If we are inside controllable window then integrate the error
                if( abs(pidError) < PID_INTEGRAL_LIMIT )
                    pidIntegral = pidIntegral + pidError;
                else
                    pidIntegral = 0;
                }
            else
                pidIntegral = 0;

            // calculate the derivative
            pidDerivative = pidError - pidLastError;
            pidLastError  = pidError;

            // calculate drive
            pidDrive = (pid_Kp * pidError) + (pid_Ki * pidIntegral) + (pid_Kd * pidDerivative);

            // limit drive
            if( pidDrive > PID_DRIVE_MAX )
                pidDrive = PID_DRIVE_MAX;
            if( pidDrive < PID_DRIVE_MIN )
                pidDrive = PID_DRIVE_MIN;

            // send to motor
            motor[ PID_MOTOR_INDEX ] = pidDrive * PID_MOTOR_SCALE;
            }
        else
            {
            // clear all
            nMotorEncoder[ PID_SENSOR_INDEX ] = 0;
            pidError      = 0;
            pidLastError  = 0;
            pidIntegral   = 0;
            pidDerivative = 0;
            motor[ PID_MOTOR_INDEX ] = 0;
            displayString(0,"DONE");
            }

        // Run at 50Hz
        wait1Msec( 25 );
        }
}

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*  main task                                                                  */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

task main()
{
	// send the motor off somewhere
  pidRequestedValue = 90;

  // start the PID task
  startTask( pidController );

}
