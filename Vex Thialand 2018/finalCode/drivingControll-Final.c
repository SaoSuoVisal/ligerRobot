#pragma config(Sensor, in1,    potentiometerScissorsLeft, sensorPotentiometer)
#pragma config(Sensor, in2,    potentiometerScissorsRight, sensorPotentiometer)
#pragma config(Sensor, in3,    potentiometerAngle, sensorPotentiometer)
#pragma config(Sensor, dgtl11, shaftEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port1,           backRight,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           frontLeft,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           frontRight,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           scissorsRight, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           scissorsLeft,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           mobileGoalLift, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           anglePivot,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           claw,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          backLeft,      tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

	int threshold = 10; 		// torelance between each joystick
	int encoderValue = 0;		// encoder variable
	int driveSpeed = 100;

// PID Variables
	int setpoint =  SensorValue[potentiometerAngle]; // 1600 //480
	int speedLimit = 100;
	int vector = 1;

	float kP = 0.12;
	float kI = 0.11;

	int err = 0;
	int currentpot = 0;

	int valueP = 0;
	int valueI = 0;

	int output = 0;
//////////////

	int conL = 0;
	int conR = 0;

	int clawSpeed = 120;
	int stalSpeed = 20;
	int out = 0;

// Function declearing
	void variableRun();
	void wheelsRun();
	void scissorsRun();
	void mobileGoalRun();
	void clawRun();
	void armRun();

// Task declearing
	task pidArm();

task main()
{
	startTask(pidArm);
	while(true){
		//Input settingup
		variableRun();

		// Y component, X component, Rotation (MASTER CONTROLLER)
		// BTN: Ch4, Ch2, Ch1
		wheelsRun();

		// Scissors Lift (PARTNER CONTROLLER)
		// BTN: 5U, 5D, 8U, 8D
		scissorsRun();

		// MobileGoal Lift	(MASTER CONTROLLER)
		// BTN: 7U, 7D, 5U, 5D
		mobileGoalRun();

		//Claw	(MASTER CONTROLLER)
		// BTN: 8U, 8D, 6U, 6D
		clawRun();

		// Arm (PARTNER CONTROLLER)
		// BTN: 7U, 7L, 7D, 7R
		armRun();
	}
}

// Tasks
task pidArm(){
	while(true){
		currentpot = SensorValue[potentiometerAngle];
		err =  -1 * (setpoint - currentpot);

		valueP = kP * err;
		valueI = kI * (valueI + err);

		output = valueP + valueI;

		if(output > speedLimit) output = speedLimit;
		else if(output < -speedLimit) output = -speedLimit;
		if(abs(output) > 5 && setpoint < 2000){
			vector = output / abs(output);
			output = output +   vector * 7;
		}
	}
}
// Functions

void motorRight(int speed){
	motor[frontRight] 	=  speed;
	motor[backRight]	=  speed;
}

void motorLeft(int speed){
	motor[frontLeft] 	=  speed;
	motor[backLeft]	=  speed;
}

void variableRun(){
	encoderValue = abs(SensorValue(shaftEncoder));

	//get input into axis
	conL = vexRT[Ch3];
	conR = vexRT[Ch2];

	//get input into axis from controller

	if(abs(conL) < threshold)	conL = 0;
	else if(conL > driveSpeed) conL = driveSpeed;
	else if(conL < -driveSpeed) conL = -driveSpeed;

	if(abs(conR) < threshold)	conR = 0;
	else if(conR > driveSpeed) conR = driveSpeed;
	else if(conR < -driveSpeed) conR = -driveSpeed;
}

void wheelsRun(){
	motorLeft(conL);
	motorRight(conR);
}

void scissorsRun(){
	int rightPot = SensorValue(potentiometerScissorsRight);
	int leftPot = SensorValue(potentiometerScissorsLeft);

	if(vexRT[Btn6UXmtr2] == 1){	//&&leftPot<1400&&rightPot<1400
		motor[scissorsLeft] = 120;
		motor[scissorsRight] = 120-(rightPot - leftPot-50)/2;
		} else if(vexRT[Btn6DXmtr2] == 1){//&&leftPot>600&&rightPot>600
		motor[scissorsLeft] = -40;
		motor[scissorsRight] = -40-(rightPot - leftPot-50)/2;
		} else{
		motor[scissorsRight] = 25-(rightPot - leftPot-50)/2;
		motor[scissorsLeft] = 25;
	}
}

void mobileGoalRun(){
	if(vexRT[Btn5D] == 1&& encoderValue < 103){
		motor[mobileGoalLift] = 100;
		} else if(vexRT[Btn5U] == 1 && encoderValue > 35){
		motor[mobileGoalLift] = -100;
		} else{
		motor[mobileGoalLift] = 20;
	}
}

void clawRun(){
	if(vexRT[Btn6D] == 1 || vexRT[Btn5DXmtr2] == 1){
		out = 1;
		motor[claw] = clawSpeed;
		} else if(vexRT[Btn6U] == 1 || vexRT[Btn5DXmtr2] == 1){
		out = 0;
		} else if(out == 1){
		motor[claw] = stalSpeed;
		} else{
		motor[claw] = 0;
	}
}

void armRun(){
	if (vexRT[Btn7UXmtr2]){
		setpoint = 4020;
	}
	if (vexRT[Btn7DXmtr2]){
		setpoint = 480;
	}
	if (vexRT[Btn7LXmtr2]){
		setpoint = 1500;
	}
	if (vexRT[Btn7RXmtr2]){
		setpoint = 1500;
	}

	motor[anglePivot] = output;
}
