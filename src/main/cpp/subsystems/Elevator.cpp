#include "math.h"
#include "Robot.h"
#include "GamepadMap.h"
#include "ctre/Phoenix.h"
#include "subsystems/Elevator.h"
#include <frc/smartdashboard/SmartDashboard.h>

#define DEADZONE_CONST  .08
#define KSLOTIDX        0
#define KPIDLOOPIDX     0
#define KTIMEOUTMS      0

#define MAX_SPEED 3660

static int speedarray[5]={0,0,0,0,0}; //array of 5 to get ticks/100ms speed
static int maxspeed = 0; //record of fastest speed here

const int Elevator::TOP_VALUE 		= 30000;
const int Elevator::BOT_VALUE       = 0;
const int Elevator::CARGO_VALUE 	= 15000;
const int Elevator::ROCKET_VALUE 	= 20000;

Elevator::Elevator() : frc::Subsystem("Elevator")
{
	SmartDashboard::PutNumber("RequestedEncoderVal", 0);
	elevatorTalonSRX = new TalonSRX(3); //itz 3 on DA can bus
	//elevatorTalonSRX->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0 , KTIMEOUTMS);
	//elevatorTalonSRX->
}
void Elevator::InitDefaultCommand() {}

void Elevator::Periodic() {
	frc::SmartDashboard::PutBoolean("Limit Switch Top"   ,GetElevatorLimitSwitchTop());
	frc::SmartDashboard::PutBoolean("Limit Switch Bottom",GetElevatorLimitSwitchBot());
	if( GetElevatorLimitSwitchBot() ) ClearElevatorEncoder();


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Manual Elevator Control~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	// double yL = Robot::m_oi->OperatorGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_Y);
	// if(fabs(yL) <= DEADZONE_CONST) yL = 0;	//Deadzone code
	// elevatorTalonSRX->Set(ControlMode::PercentOutput, yL *(-1.0));
	//Position mode - button just pressed
	//double reqValue = SmartDashboard::GetNumber("RequestedEncoderVal", 0);
	
	frc::SmartDashboard::PutNumber("ElevatorEncoderVal", GetElevatorEncoder()*(-1.0));
	frc::SmartDashboard::PutNumber("MotorOutputPct", elevatorTalonSRX->GetMotorOutputPercent());
	frc::SmartDashboard::PutNumber("ClosedLoopError", elevatorTalonSRX->GetClosedLoopError(KPIDLOOPIDX));
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Mr. B's elevator Speed Test~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//This code can be commented out once we do it, and the manual code above can be uncommented
	//static double reqValue = 0;
	double throttle = Robot::m_oi->OperatorGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_Y);
	//static int speedarray[5]={0,0,0,0,0}; //array of 5 to get ticks/100ms speed
	//static int maxspeed = 0; //record of fastest speed here
		int idx = 0;
		//double povAngle = Robot::m_oi->DriverGamepad()->GetPOV(0);
		//static bool povCenter = false;
	if (Robot::m_oi->OperatorGamepad()->GetRawButton(1))
	{
		//SmartDashboard::PutNumber("DriverPOVAngle",povAngle);
		//reqValue = 28000;
		throttle = throttle * MAX_SPEED;
		elevatorTalonSRX->Set (ControlMode::Velocity, throttle);

		for (idx = 0; idx <= 3; idx++)
		{ 
			speedarray[idx] = speedarray[idx+1];
		}
		speedarray[4] = (-1)*GetElevatorEncoder();
		if (abs(speedarray[4]) - abs(speedarray[0]) > maxspeed) {maxspeed = abs(speedarray[4]) - abs(speedarray[0]);}

		SmartDashboard::PutNumber("MaxElevatorSpeed",maxspeed);
		SmartDashboard::PutNumber("Speed Array 4",speedarray[4]);
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END OF MR.B'S CODE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




	// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~Operator Gamepad POV for elevator control~~~~~~~~~~~~~~~~~~~~~~~
	// //0 = Up, 90 = Right, 180 = Down, 270 = Left
	// double povAngle = Robot::m_oi->OperatorGamepad()->GetPOV(0);
	// static bool povCenter = false;

	// SmartDashboard::PutNumber("OperatorPOVAngle",povAngle);

	// if (povAngle == -1.0)	povCenter = true;

	// if ((povAngle == 0.0) && povCenter ){
	// 	std::cout<<"Top"<<std::endl;
	// 	reqValue = 30000;
	// 	povCenter = false;
	// }

	// if ((povAngle == 90.0) && povCenter ){
	// 	std::cout<<"Middle High"<<std::endl;
	// 	reqValue = 20000;
	// 	povCenter = false;
	// }
	// if ((povAngle == 180.0) && povCenter ){
	// 	std::cout<<"Bottom"<<std::endl;
	// 	reqValue = 0;
	// 	povCenter = false;
	// }
	// if ((povAngle == 270.0) && povCenter ){
	// 	std::cout<<"Middle Low"<<std::endl;
	// 	reqValue = 15000;
	// 	povCenter = false;
	// }

	// //elevatorTalonSRX->Set (ControlMode::Position, reqValue); 


}

void Elevator::TalonSRXinit(void)
{
    //int KTIMEOUTMS = 0;

    elevatorTalonSRX->ConfigForwardLimitSwitchSource(
		LimitSwitchSource::LimitSwitchSource_FeedbackConnector,
		LimitSwitchNormal::LimitSwitchNormal_NormallyOpen,
		KTIMEOUTMS);

    elevatorTalonSRX->ConfigReverseLimitSwitchSource(
    	LimitSwitchSource::LimitSwitchSource_FeedbackConnector,
    	LimitSwitchNormal::LimitSwitchNormal_NormallyOpen,
    	KTIMEOUTMS);

    elevatorTalonSRX->SetNeutralMode(NeutralMode::Brake);

    elevatorTalonSRX->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, KTIMEOUTMS);

	//int absolutePosition = elevatorTalonSRX->GetSelectedSensorPosition(0) & 0xFFF;
	elevatorTalonSRX->SetSelectedSensorPosition(0, KPIDLOOPIDX, KTIMEOUTMS);
	
	elevatorTalonSRX->SetSensorPhase(true);

	//set the peak and nominal outputs, 12V means full
	elevatorTalonSRX->ConfigNominalOutputForward(0, KTIMEOUTMS);
	elevatorTalonSRX->ConfigNominalOutputReverse(0, KTIMEOUTMS);
	elevatorTalonSRX->ConfigPeakOutputForward(1.0, KTIMEOUTMS);
	elevatorTalonSRX->ConfigPeakOutputReverse(-1.0, KTIMEOUTMS);

	//set closed loop gains in slot0
	// elevatorTalonSRX->Config_kF(KPIDLOOPIDX, 0.0, KTIMEOUTMS);
	// elevatorTalonSRX->Config_kP(KPIDLOOPIDX, 0.3, KTIMEOUTMS);
	// elevatorTalonSRX->Config_kI(KPIDLOOPIDX, 0.0, KTIMEOUTMS);
	// elevatorTalonSRX->Config_kD(KPIDLOOPIDX, 0.0, KTIMEOUTMS);

    elevatorTalonSRX->SetInverted(false);
}

void Elevator::ElevatorMotorDrive(double power){
	elevatorTalonSRX->Set(ControlMode::PercentOutput, power);
}
void Elevator::Stop(void){
	elevatorTalonSRX->Set(ControlMode::PercentOutput, 0.0);
	
}
int  Elevator::GetElevatorEncoder(void){
	return elevatorTalonSRX->GetSensorCollection().GetQuadraturePosition();
}

bool Elevator::GetElevatorLimitSwitchTop(void){
	return elevatorTalonSRX->GetSensorCollection().IsFwdLimitSwitchClosed();
}

bool Elevator::GetElevatorLimitSwitchBot(void){
	return elevatorTalonSRX->GetSensorCollection().IsRevLimitSwitchClosed();
}
void Elevator::ClearElevatorEncoder(void){
	elevatorTalonSRX->SetSelectedSensorPosition(0,0,0);
}
double Elevator::GetElevatorMotorDrive(void){
	return elevatorTalonSRX->GetMotorOutputPercent();
}
void Elevator::SetReqElevatorValue( int value ){
	elevatorTalonSRX->Set (ControlMode::Position, value); 
}