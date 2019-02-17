#include "subsystems/Collector.h"
#include "Robot.h"
#include "OI.h"
#include "GamepadMap.h" 

#define DEADBAND_CONST  .08

const int Collector::RETRACT_CAGE = 0;
const int Collector::DEPLOY_CAGE = 1;
const int Collector::CLOSE_BRIDGE = 0;
const int Collector::OPEN_BRIDGE = 1;
const double Collector::COLLECT_SPEED = 0.3;
const double Collector::BRIDGE_SPEED = 0.2;

const double Collector::MANUAL_SPIT_SPEED = -1.0;
const double Collector::MANUAL_INTAKE_FAST = 0.7;  //May want to change 
const double Collector::MANUAL_INTAKE_SLOW = 0.4;

Collector::Collector() : Subsystem("Collector")
{ 
    collectorCage = new frc::DoubleSolenoid(1,2,3);
	collectorBridge = new frc::DoubleSolenoid(1,6,7);
    collectorPhotoeye = new frc::DigitalInput(7);
    collectorRollers =  new VictorSPX(14); 
    //m_CageDeploy = false;
}

void Collector::InitDefaultCommand() {}

void Collector::CollectorPeriodic(void)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Bridge Open/Close Toggle~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool startPressed = Robot::m_oi->OperatorGamepad()->GetRawButtonPressed(GAMEPADMAP_BUTTON_START); 
	//This is bens awful looking toggle attempt
	
	if(startPressed)
	{
		if(IsBridgeDeployed())
			CloseBridge();
		else
			OpenBridge();
	}
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Manual Collector Roller Control~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	double manualIntakeThrottle = Robot::m_oi->OperatorGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_TRIG); 
	if(manualIntakeThrottle >= 0.5)
	{
		CollectorRollers(MANUAL_SPIT_SPEED);
	}
	//if right bumper pressed go fast, if back and right bumper pressed go slower? idk why we need this it seems dumb -Ben L.
	else if(Robot::m_oi->OperatorGamepad()->GetRawButton(GAMEPADMAP_BUTTON_RBUMP)) 
	{
		CollectorRollers(MANUAL_INTAKE_FAST);
	}
	else if (Robot::m_oi->OperatorGamepad()->GetRawButton(GAMEPADMAP_BUTTON_RBUMP) && Robot::m_oi->OperatorGamepad()->GetRawButton(GAMEPADMAP_BUTTON_BACK)) 
	{
		CollectorRollers(MANUAL_INTAKE_SLOW);
	}
	else
	{
		CollectorRollers(0.0);
	}
	//*********Auto Collector Control***********







	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Manual Collector Control~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	double yR = Robot::m_oi->OperatorGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_Y);
	if(fabs(yR) <= DEADBAND_CONST) yR = 0;	//Deadband code
	//if joystick pushed up, cage goes up
	//if joystick pushed down, cage goes down
	if(yR >= 0.5) DeployCage();
	if(yR <= -0.5) RetractCage();
}

//********************************************END OF PERIODIC**********************************************************


//********PhotoEye***********
bool Collector::IsPhotoeyeDetected(void)
{
	return collectorPhotoeye->Get();
}
//********CAGE***********
void Collector::DeployCage(void)
{
	collectorCage->Set(DoubleSolenoid::kForward);
	m_DeployCage = true;
}
void Collector::RetractCage(void)
{
	collectorCage->Set(DoubleSolenoid::kReverse);
	m_DeployCage = false;
}
void Collector::SetCage(int position)
{
	if(position == DEPLOY_CAGE)   DeployCage();
	if(position == RETRACT_CAGE)  RetractCage();
}
bool Collector::IsCageDeployed(void){
	return m_DeployCage;
}
//********BRIDGE***********
void Collector::OpenBridge(void)
{
	collectorBridge->Set(DoubleSolenoid::kForward);
	m_OpenBridge = true;
}
void Collector::CloseBridge(void)
{
	collectorBridge->Set(DoubleSolenoid::kReverse);
	m_OpenBridge = false;
}
void Collector::SetBridge(int position)
{
	if(position == OPEN_BRIDGE)   OpenBridge();
	if(position == CLOSE_BRIDGE)  CloseBridge();
}
bool Collector::IsBridgeDeployed(void){
	return m_OpenBridge;
}
//********ROLLERS***********
void Collector::CollectorRollers(double power)
{
	collectorRollers->Set(ControlMode::PercentOutput, power);
}
void Collector::StopCollectorRollers(void)
{
	collectorRollers->Set(ControlMode::PercentOutput, 0.0);
}

// Victor Init of Rollers
void Collector::VictorSPXInit(void)
{
	collectorRollers->ConfigFactoryDefault();

	collectorRollers->SetInverted(true);

	collectorRollers->SetNeutralMode(NeutralMode::Brake);

}