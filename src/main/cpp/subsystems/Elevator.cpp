#include "math.h"
#include "Robot.h"
#include "GamepadMap.h"
#include "ctre/Phoenix.h"
#include "subsystems/Elevator.h"
#include <frc/smartdashboard/SmartDashboard.h>

#define DEADZONE_CONST  .2 //was .08
#define KSLOTIDX        0
#define KPIDLOOPIDX     0
#define KTIMEOUTMS      0

const int Elevator::BOT_VALUE         = 0;
const int Elevator::CARGO_VALUE 	  = 5486;
const int Elevator::ROCKET_MID_VALUE  = 13253;
const int Elevator::ROCKET_HIGH_VALUE = 26244;
const double Elevator::UP_DRIVE       = 1.0;
const double Elevator::DOWN_DRIVE     = -0.2; //was -0.1
const double Elevator::HOLD_DRIVE     = 0.15; //was 0.1
const int Elevator::ENCODER_TOLERANCE = 484; //was 242

Elevator::Elevator() : frc::Subsystem("Elevator")
{
	elevatorTalonSRX = new TalonSRX(3); //itz 3 on DA can bus
	elevatorVictorSPX = new VictorSPX(4);
	int reqValue = 0;
	elevatorTalonSRX->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0 , KTIMEOUTMS);
}
void Elevator::InitDefaultCommand() {
	
}

void Elevator::ElevatorPeriodic() {
	frc::SmartDashboard::PutBoolean("Limit Switch Top"   ,GetElevatorLimitSwitchTop());
	frc::SmartDashboard::PutBoolean("Limit Switch Bottom",GetElevatorLimitSwitchBot());
	if( GetElevatorLimitSwitchBot() ) ClearElevatorEncoder();

	frc::SmartDashboard::PutNumber("ElevatorEncoderVal", GetElevatorEncoder());
	frc::SmartDashboard::PutNumber("ElevatorOutputPct", elevatorTalonSRX->GetMotorOutputPercent());
    frc::SmartDashboard::PutNumber("ElevatorRequestedVal", reqValue);
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~Operator Gamepad POV for elevator control~~~~~~~~~~~~~~~~~~~~~~~
	//0 = Up, 90 = Right, 180 = Down, 270 = Left
    double povAngle = Robot::m_oi->OperatorGamepad()->GetPOV(0);
	static bool povCenter = false;

	 SmartDashboard::PutNumber("OperatorPOVAngle",povAngle);

	 if (povAngle == -1.0)	povCenter = true;

	 if ((povAngle == 0.0) && povCenter ){
	 	std::cout<<"Top"<<std::endl;
	 	reqValue = ROCKET_HIGH_VALUE;
	 	povCenter = false;
	 }

	 if ((povAngle == 90.0) && povCenter ){
	 	std::cout<<"Rocket Ship"<<std::endl;
	 	reqValue = ROCKET_MID_VALUE;
	 	povCenter = false;
	 }
	 if ((povAngle == 180.0) && povCenter ){
	 	std::cout<<"Bottom"<<std::endl;
	 	reqValue = BOT_VALUE;
	 	povCenter = false;
	 }
     if ((povAngle == 270.0) && povCenter ){
	 	std::cout<<"Cargo Ship"<<std::endl;
	 	reqValue = CARGO_VALUE;
	 	povCenter = false;
	 }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Elevator Control~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	double yL = Robot::m_oi->OperatorGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_Y);
	if(fabs(yL) <= DEADZONE_CONST) yL = 0;	//Deadzone code
    int currValue = GetElevatorEncoder();
	//Elevator Manual Override
    if(Robot::m_oi->OperatorGamepad()->GetRawButton(GAMEPADMAP_BUTTON_A))
    { 
        elevatorTalonSRX->Set(ControlMode::PercentOutput, yL *(-1.0));
		reqValue = GetElevatorEncoder();
    } 
   else //THIS is where the code gets good (we move the elevator based on tolerance error)
   {
        if( (currValue < (reqValue - ENCODER_TOLERANCE) ) || (currValue > (reqValue + ENCODER_TOLERANCE)))
        {
            if(currValue < reqValue)
                elevatorTalonSRX->Set(ControlMode::PercentOutput, UP_DRIVE);
            if(currValue > reqValue)
            {
                std::cout<<"down drive"<<std::endl;
                elevatorTalonSRX->Set(ControlMode::PercentOutput, DOWN_DRIVE);
            }
        }
        else  
            {
            std::cout<<"hold drive"<<std::endl;
            elevatorTalonSRX->Set(ControlMode::PercentOutput, HOLD_DRIVE);
            }
   }
}
void Elevator::TalonSRXinit(void)
{
	elevatorTalonSRX->ConfigFactoryDefault();
	elevatorVictorSPX->ConfigFactoryDefault();

    elevatorTalonSRX->ConfigForwardLimitSwitchSource(
		LimitSwitchSource::LimitSwitchSource_FeedbackConnector,
		LimitSwitchNormal::LimitSwitchNormal_NormallyOpen,
		KTIMEOUTMS);

    elevatorTalonSRX->ConfigReverseLimitSwitchSource(
    	LimitSwitchSource::LimitSwitchSource_FeedbackConnector,
    	LimitSwitchNormal::LimitSwitchNormal_NormallyOpen,
    	KTIMEOUTMS);

    elevatorTalonSRX->SetNeutralMode(NeutralMode::Brake);
	elevatorVictorSPX->SetNeutralMode(NeutralMode::Brake);

    elevatorTalonSRX->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, KTIMEOUTMS);

	//int absolutePosition = elevatorTalonSRX->GetSelectedSensorPosition(0) & 0xFFF;
	elevatorTalonSRX->SetSelectedSensorPosition(0, KPIDLOOPIDX, KTIMEOUTMS);
	
	//set the peak and nominal outputs, 12V means full
	elevatorTalonSRX->ConfigNominalOutputForward(0, KTIMEOUTMS);
	elevatorTalonSRX->ConfigNominalOutputReverse(0, KTIMEOUTMS);
	elevatorTalonSRX->ConfigPeakOutputForward(1.0, KTIMEOUTMS);
	elevatorTalonSRX->ConfigPeakOutputReverse(-1.0, KTIMEOUTMS);

	elevatorTalonSRX->SetSensorPhase(true); //was false
    elevatorTalonSRX->SetInverted(false);
	elevatorVictorSPX->SetInverted(true);

	elevatorVictorSPX->Follow(*elevatorTalonSRX);
}

void Elevator::ElevatorMotorDrive(double power){
	elevatorTalonSRX->Set(ControlMode::PercentOutput, power);
}
void Elevator::Stop(void){
	elevatorTalonSRX->Set(ControlMode::PercentOutput, 0.0);
	
}
int  Elevator::GetElevatorEncoder(void){
	return elevatorTalonSRX->GetSensorCollection().GetQuadraturePosition()*(-1.0);
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
void Elevator::ClearReqElevatorValue(void){
	reqValue = 0; 
}