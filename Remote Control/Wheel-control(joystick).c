#pragma config(Motor,  port1,           rightMotor,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port10,          leftMotor,     tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{

	while(1 == 1){
	//Wheels remote control
		motor [rightMotor] = vexRT[Ch2]; //channel 3 controlling left motor
		motor [leftMotor] = vexRT[Ch2]; //channel 2 controlling right motor

		motor[leftMotor] = vexRT(Ch1);
		motor[rightMotor] = vexRT(Ch1)*-1;
	//End
	}
}
