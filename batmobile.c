#pragma config(Sensor, in1,    steeringPot,    sensorPotentiometer)
#pragma config(Sensor, in2,    brakePedalPot,  sensorPotentiometer)
#pragma config(Sensor, in3,    gasPedalPot,    sensorPotentiometer)
#pragma config(Sensor, in4,    linearActuatorPot, sensorPotentiometer)
#pragma config(Sensor, in5,    shiftStickPosition, sensorAnalog)
#pragma config(Sensor, dgtl1,  shiftHighDio,   sensorDigitalIn)
#pragma config(Sensor, dgtl2,  shiftLowDio,    sensorDigitalIn)
#pragma config(Sensor, dgtl3,  shiftReverseDio, sensorDigitalIn)
#pragma config(Sensor, dgtl4,  driftDio,       sensorDigitalIn)
#pragma config(Sensor, dgtl5,  shiftGreenOut,  sensorDigitalOut)
#pragma config(Sensor, dgtl6,  shiftRedOut,    sensorDigitalOut)
#pragma config(Sensor, dgtl7,  shiftBlueOut,   sensorDigitalOut)
#pragma config(Sensor, dgtl8,  controlModeLightOut, sensorDigitalOut)
#pragma config(Sensor, dgtl9,  auxiliaryLightingOne, sensorDigitalOut)
#pragma config(Sensor, dgtl10, auxiliaryLightingTwo, sensorDigitalOut)
#pragma config(Sensor, dgtl11, controlSchemeBitTwo, sensorDigitalIn)
#pragma config(Sensor, dgtl12, controlSchemeBitOne, sensorDigitalIn)
#pragma config(Motor,  port2,           rightFrontMotor, tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port3,           leftFrontMotor, tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port4,           rearMotor,     tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port5,           turnLinearActuator, tmotorServoContinuousRotation, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define LINEAR_ACTUATOR_MID 1980
#define LINEAR_ACTUATOR_MAX_RIGHT 820
#define LINEAR_ACTUATOR_MAX_LEFT 3139
#define LINEAR_ACTUATOR_TICKS_PER_ONE_JOYSTICK 10
#define LINEAR_ACTUATOR_JOYSTICK_THRESHOLD 10
#define LINEAR_ACTUATOR_POT_THRESHOLD 40

#define SHIFT_REVERSE_GEAR 0
#define SHIFT_LOW_GEAR 1
#define SHIFT_HIGH_GEAR 2
#define SHIFT_REVERSE_HIGH_GEAR 3

#define CONTROL_SCHEME_1 0
#define CONTROL_SCHEME_2 1
#define CONTROL_SCHEME_3 2
#define CONTROL_SCHEME_4 3

#define KID_CONTROL 0
#define PARENT_CONTROL 1

#define BOOST_OFF 0
#define BOOST_ON 1

#define DRIFT_OFF 0
#define DRIFT_ON 1

#define REVERSE_GEAR_MAX_SPEED -64
#define LOW_GEAR_MAX_SPEED 64
#define HIGH_GEAR_MAX_SPEED 108
#define REVERSE_HIGH_GEAR_MAX_SPEED -108

#define BOOST_REVERSE_GEAR_MAX_SPEED -64
#define BOOST_LOW_GEAR_MAX_SPEED 74
#define BOOST_HIGH_GEAR_MAX_SPEED 127
#define BOOST_REVERSE_HIGH_GEAR_MAX_SPEED -127

#define DRIFT_REAR_WHEEL_SPEED 0

#define THROTTLE_SCALE_FACTOR 3

#define CONTROL_MODE_LIGHT_PERIOD 13

bool revertToParentMode = false;
bool btn8LLastPressed = false;
bool btn8ULastPressed = false;

/*----------------------------------------------------------------------------------------------------*\
|*                            - Single Joystick Control with Thresholds -                             *|
|*                                      ROBOTC on VEX 2.0 Cortex                                      *|
|*                                                                                                    *|
|*  This program uses both the Left and the Right joysticks to run the robot using "tank control".    *|
|*  It also uses a threshold value to ignore low readings from the joystick.  This eliminates         *|
|*  "noise" from joysticks that have not returned exactly to (0,0) which is common.                   *|
|*                                                                                                    *|
|*                                        ROBOT CONFIGURATION                                         *|
|*    NOTES:                                                                                          *|
|*    1)  Ch1 is the X axis and Ch2 is the Y axis for the RIGHT joystick.                             *|
|*    2)  Ch3 is the Y axis and Ch4 is the X axis for the LEFT joystick.                              *|
|*    3)  'abs(z)' is the ABSOLUTE VALUE of 'z'.                                                      *|
|*                                                                                                    *|
|*    MOTORS & SENSORS:                                                                               *|
|*    [I/O Port]              [Name]              [Type]              [Description]                   *|
|*    Motor - Port 2          rightMotor           VEX Motor           Right motor                    *|
|*    Motor - Port 3          leftMotor            VEX Motor           Left motor                     *|
\*----------------------------------------------------------------------------------------------------*/

int clipMotorOutput(int output, int max) {
	if(output > 0 && output > max) {
		output = max;
	}
	else if(output < 0 && output < max) {
		output = max;
	}
	//else if(output == 0) {
	//	output = max;
	//}

	return output;
}

int rampDown(int currentSpeed) {
	if(currentSpeed > 0) {
		currentSpeed -= THROTTLE_SCALE_FACTOR;
	}
	else if(currentSpeed < 0) {
		currentSpeed += THROTTLE_SCALE_FACTOR;
	}

	return currentSpeed;
}

void driveLinearActuator(int steer) {
	int linearActuatorPotValue = SensorValue[linearActuatorPot];
	int linearActuatorSetpointFromCenter = abs(steer) * LINEAR_ACTUATOR_TICKS_PER_ONE_JOYSTICK;
	int linearActuatorSetpoint;

	if(steer > LINEAR_ACTUATOR_JOYSTICK_THRESHOLD) {
		linearActuatorSetpoint = LINEAR_ACTUATOR_MID - linearActuatorSetpointFromCenter;
		if(linearActuatorSetpoint < LINEAR_ACTUATOR_MAX_RIGHT) {
			linearActuatorSetpoint = LINEAR_ACTUATOR_MAX_RIGHT;
		}
	}
	else if(steer < -LINEAR_ACTUATOR_JOYSTICK_THRESHOLD) {
		linearActuatorSetpoint = LINEAR_ACTUATOR_MID + linearActuatorSetpointFromCenter;
		if(linearActuatorSetpoint > LINEAR_ACTUATOR_MAX_LEFT) {
			linearActuatorSetpoint = LINEAR_ACTUATOR_MAX_LEFT;
		}
	}
	else {
		linearActuatorSetpoint = LINEAR_ACTUATOR_MID;
	}

	if(linearActuatorPotValue - linearActuatorSetpoint > LINEAR_ACTUATOR_POT_THRESHOLD) {
		motor[turnLinearActuator] = 127; //75
	}
	else if(linearActuatorPotValue - linearActuatorSetpoint < -LINEAR_ACTUATOR_POT_THRESHOLD) {
		motor[turnLinearActuator] = -127; //-75
	}
	else {
		motor[turnLinearActuator] = 0;
	}
}

int getStickShiftPosition() {
	// Shifter
	int shiftStickValue = SensorValue[shiftStickPosition];
	int retVal = SHIFT_LOW_GEAR;

	// LED values on the digital outs are reversed for the shifter, 0 turns a color on,
	// 1 turns it off.
	if(800 > shiftStickValue && shiftStickValue > 700) {
		// shifter in reverse
		SensorValue[shiftRedOut] = 0;
		SensorValue[shiftBlueOut] = 1;
		SensorValue[shiftGreenOut] = 1;

		retVal = SHIFT_REVERSE_GEAR;
	}
	else if(1550 > shiftStickValue && shiftStickValue > 1450) {
		// shifter in high
		SensorValue[shiftBlueOut] = 0;
		SensorValue[shiftGreenOut] = 1;
		SensorValue[shiftRedOut] = 1;

		retVal = SHIFT_HIGH_GEAR;
  }
  else if(1800 > shiftStickValue && shiftStickValue > 1700) {
  	// shifter in low
  	SensorValue[shiftGreenOut] = 0;
  	SensorValue[shiftBlueOut] = 1;
		SensorValue[shiftRedOut] = 1;

		retVal = SHIFT_LOW_GEAR;
  }

  return retVal;
}

int getSteeringWheelPosition() {
	int retVal;

	float steeringPotValue = SensorValue[steeringPot];
	float centeredValue = steeringPotValue - 1990;

	if(fabs(centeredValue) < 100) {
		retVal = 0;
	}
	else {
		retVal = centeredValue / 4.72;
	}

	return round(retVal);
}

void driveMotors(int speed, int boostMode, int driftMode, int stickShiftPosition) {
	int threshold = 10;   // helps to eliminate 'noise' from a joystick that isn't perfectly at (0,0)
                        // feel free to change this to match your needs.

	switch(boostMode) {
		case BOOST_OFF:
			switch(stickShiftPosition) {
				case SHIFT_LOW_GEAR:
					speed = clipMotorOutput(speed, LOW_GEAR_MAX_SPEED);
					break;
				case SHIFT_HIGH_GEAR:
					speed = clipMotorOutput(speed, HIGH_GEAR_MAX_SPEED);
					break;
				case SHIFT_REVERSE_GEAR:
					speed = clipMotorOutput(speed, REVERSE_GEAR_MAX_SPEED);
					break;
				case SHIFT_REVERSE_HIGH_GEAR:
					speed = clipMotorOutput(speed, REVERSE_HIGH_GEAR_MAX_SPEED);
					break;
			}
			break;
		case BOOST_ON:
			switch(stickShiftPosition) {
					case SHIFT_LOW_GEAR:
						speed = clipMotorOutput(speed, BOOST_LOW_GEAR_MAX_SPEED);
						break;
					case SHIFT_HIGH_GEAR:
						speed = clipMotorOutput(speed, BOOST_HIGH_GEAR_MAX_SPEED);
						break;
					case SHIFT_REVERSE_GEAR:
						speed = clipMotorOutput(speed, BOOST_REVERSE_GEAR_MAX_SPEED);
					case SHIFT_REVERSE_HIGH_GEAR:
						speed = clipMotorOutput(speed, BOOST_REVERSE_HIGH_GEAR_MAX_SPEED);
				}
			break;
	}

	int currentSpeed = motor[leftFrontMotor]; //all motors should be at the same speed unless we're drifting

  if(abs(speed) > threshold) {
  	if(speed < 0) {
  			speed = clipMotorOutput(speed, currentSpeed - THROTTLE_SCALE_FACTOR);
  	}
  	else if(speed > 0) {
  		speed = clipMotorOutput(speed, currentSpeed + THROTTLE_SCALE_FACTOR);
  	}

  	motor[leftFrontMotor] = speed;
  	motor[rightFrontMotor] = speed;

  	int rearSpeed = speed;
  	switch(driftMode) {
			case DRIFT_OFF:
				break;
			case DRIFT_ON:
				rearSpeed = clipMotorOutput(rearSpeed, DRIFT_REAR_WHEEL_SPEED);
				break;
		}
		motor[rearMotor] = rearSpeed;
  }
  else {
  	speed = rampDown(currentSpeed);

  	if(abs(speed) < threshold) {
  		speed = 0;
  	}

 		motor[leftFrontMotor] = speed;
 		motor[rightFrontMotor] = speed;
 		motor[rearMotor] = speed;
  }
}

void driveControlSchemeOne(int boostMode, int driftMode) {
	//steering Ch4
	//throttle 8R
	//brake 8D
	int steer = vexRT[Ch4];
	int speed = 0;
	if(vexRT[Btn8R] == 1) {
		speed = 127;
	}
	else if(vexRT[Btn8D] == 1) {
		speed = -127;
	}
	else {
		speed = 0;
	}

	driveMotors(speed, boostMode, driftMode, speed >= 0 ? SHIFT_HIGH_GEAR : SHIFT_REVERSE_HIGH_GEAR);
	driveLinearActuator(steer);
}

void driveControlSchemeTwo(int boostMode, int driftMode) {
	//steering Ch1
	//throttle Ch3
	int steer = vexRT[Ch1];
	int speed = vexRT[Ch3];

	driveMotors(speed, boostMode, driftMode, speed >= 0 ? SHIFT_HIGH_GEAR : SHIFT_REVERSE_HIGH_GEAR);
	driveLinearActuator(steer);
}

void driveControlSchemeThree(int boostMode, int driftMode) {
	//steering Ch4
	//throttle Ch3
	int steer = vexRT[Ch4];
	int speed = vexRT[Ch3];

	driveMotors(speed, boostMode, driftMode, speed >= 0 ? SHIFT_HIGH_GEAR : SHIFT_REVERSE_HIGH_GEAR);
	driveLinearActuator(steer);
}

void driveControlSchemeFour(int boostMode, int driftMode) {
	//steering Ch4
	//throttle Ch2
	int steer = vexRT[Ch4];
	int speed = vexRT[Ch2];

	driveMotors(speed, boostMode, driftMode, speed >= 0 ? SHIFT_HIGH_GEAR : SHIFT_REVERSE_HIGH_GEAR);
	driveLinearActuator(steer);
}

void driveControlSchemeKid(int stickShiftPosition, int boostMode, int driftMode) {
	float brakePedalValue = SensorValue[brakePedalPot];
  float gasPedalValue = SensorValue[gasPedalPot];
  int steer = getSteeringWheelPosition();
  int speed = 0;
  float potScalingFactor = 21.66;

  if(brakePedalValue > 800) {
  	// brake is pressed
  	speed = 127 - round((brakePedalValue - 800) / potScalingFactor);
	}
	else if(gasPedalValue > 800) {
		// gas is pressed
		speed = round((gasPedalValue - 800) / potScalingFactor);
		if(stickShiftPosition == SHIFT_REVERSE_GEAR) {
			speed = -speed;
		}
	}
	else {
		// neither is pressed
		speed = 0;
	}

  driveMotors(speed, boostMode, driftMode, stickShiftPosition);
  driveLinearActuator(steer);
}

void move(int controlMode, int controlScheme
											, int stickShiftPosition, int boostMode
											, int driftMode) {
  switch(controlMode) {
  	case PARENT_CONTROL:
		  switch(controlScheme) {
		  	case CONTROL_SCHEME_1:
		  		driveControlSchemeOne(boostMode, driftMode);
		  		break;
		  	case CONTROL_SCHEME_2:
		  		driveControlSchemeTwo(boostMode, driftMode);
		  		break;
		  	case CONTROL_SCHEME_3:
		  		driveControlSchemeThree(boostMode, driftMode);
		  		break;
		  	case CONTROL_SCHEME_4:
		  		driveControlSchemeFour(boostMode, driftMode);
		  		break;
		  	default:
		  		break;
		  }
		  break;
		 case KID_CONTROL:
		 	driveControlSchemeKid(stickShiftPosition, boostMode, driftMode);
		 	break;
	}
}

int getConfiguredControlScheme(){
	return 3 - (SensorValue[controlSchemeBitOne] * 1 + SensorValue[controlSchemeBitTwo] * 2);
}

int getControlMode (int currentControlMode)
{
	//remote control - 7L or 7D
	//onboard control - 7U
	//onboard control - 5U or 5D held
	int retVal;

	if(vexRT[Btn7L] == 1 || vexRT[Btn7D] == 1) {
		retVal = PARENT_CONTROL;
		revertToParentMode = false;
	}
	else if(vexRT[Btn7U] == 1) {
		retVal = KID_CONTROL;
		revertToParentMode = false;
	}
	else if(vexRT[Btn5U] == 1 || vexRT[Btn5D] == 1) {
		retVal = KID_CONTROL;
		//revert to PARENT_CONTROL regardless of what state we were in when we started
		//holding the momentary.
		revertToParentMode = true;
	}
	else if(revertToParentMode == true) {
		retVal = PARENT_CONTROL;
		revertToParentMode = false;
	}
	else {
		retVal = currentControlMode;
	}

	return retVal;
}

int getBoostMode() {
	//boost - 6U
	int retVal;

	if(vexRT[Btn6U] == 1) {
		retVal = BOOST_ON;
	}
	else {
		retVal = BOOST_OFF;
	}

	return retVal;
}

int getDriftMode() {
	//drift - 6D
	int retVal;

	if(vexRT[Btn6D] == 1) {
		retVal = DRIFT_ON;
	}
	else {
		retVal = DRIFT_OFF;
	}

	return retVal;
}

void setControlModeLight(int controlMode, int loopCount) {
	switch(controlMode) {
		case KID_CONTROL:
			if((loopCount % CONTROL_MODE_LIGHT_PERIOD) == 0) {
				SensorValue[controlModeLightOut] ^= 1;
			}
			break;
		case PARENT_CONTROL:
			SensorValue[controlModeLightOut] = 0;
			break;
		default:
			SensorValue[controlModeLightOut] = 1;
			break;
	}
}

void setAuxiliaryLightingOne(){
	if(vexRT[Btn8L] == 1 && !btn8LLastPressed) {
		SensorValue[auxiliaryLightingOne] ^= 1;
		btn8LLastPressed = true;
	}
	
	if(vexRT[Btn8L] == 0) {
		btn8LLastPressed = false;
	}
}

void setAuxiliaryLightingTwo(){
	if(vexRT[Btn8U] == 1 && !btn8ULastPressed) {
		SensorValue[auxiliaryLightingTwo] ^= 1;
		btn8ULastPressed = true;
	}
	
	if(vexRT[Btn8U] == 0) {
		btn8ULastPressed = false;
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++| MAIN |+++++++++++++++++++++++++++++++++++++++++++++++
task main ()
{
	int controlScheme;
	int stickShiftPosition;
	int controlMode = PARENT_CONTROL;
	int boostMode;
	int driftMode;
	int loopCount = 0;

	//start with auxiliary lighting on
	SensorValue[auxiliaryLightingOne] = 1;
	SensorValue[auxiliaryLightingTwo] = 1;

  while(1 == 1)
  {
  	controlMode = getControlMode(controlMode);
  	setControlModeLight(controlMode, loopCount);
  	setAuxiliaryLightingOne();
  	setAuxiliaryLightingTwo();

  	controlScheme = getConfiguredControlScheme();
  	stickShiftPosition = getStickShiftPosition();
  	boostMode = getBoostMode();
  	driftMode = getDriftMode();

  	move(controlMode, controlScheme, stickShiftPosition, boostMode, driftMode);

  	if((loopCount % (CONTROL_MODE_LIGHT_PERIOD * 100)) == 0) {
  		loopCount = 0;
  	}
  	loopCount++;

  	//sleep for 30ms to approximate a periodic tick-through
  	wait1Msec(30);
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
