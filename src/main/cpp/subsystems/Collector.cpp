#include "subsystems/Collector.h"
#include "Robot.h"
#include "OI.h"
#include "GamepadMap.h" 

#define DEADBAND_CONST  .08

const int Collector::RETRACT_CAGE = 0;
const int Collector::DEPLOY_CAGE = 1;
const int Collector::CLOSE_BRIDGE = 0;
const int Collector::OPEN_BRIDGE = 1;
const int Collector::COLLECT_SPEED = 0.3;
const int Collector::BRIDGE_SPEED = 0.2;

const int Collector::MANUAL_SPIT_SPEED = 1;
const int Collector::MANUAL_INTAKE_FAST = .7;//May want to change l8r
const int Collector::MANUAL_INTAKE_SLOW = .4;

Collector::Collector() : Subsystem("Collector")
{ 
    collectorCage = new frc::DoubleSolenoid(1,2,3);
	collectorBridge = new frc::DoubleSolenoid(1,6,7);
    collectorPhotoeye = new frc::DigitalInput(4);
    collectorMotor =  new frc::Spark(5); //should be 0
    //m_CageDeploy = false;
}

void Collector::InitDefaultCommand() {}

void Collector::CollectorPeriodic(void)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Bridge Open/Close Toggle~~~~~~~~~~~~~~~~~~~~~~~~~~
	static bool flag = false;
	static bool startPressed = Robot::m_oi->OperatorGamepad()->GetRawButton(8); //HEY magic number fix gampad map Start
	//
	if(!(IsBridgeDeployed()) && startPressed && !(flag))
	{
		flag = true;
		OpenBridge();
	}
	else if(IsBridgeDeployed() && startPressed && !(flag))
	{
		flag = true;
		CloseBridge();
	}
	else
	{
		flag = false;
	}
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Manual Spit from Collector~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	double manualIntakeThrottle = Robot::m_oi->OperatorGamepad()->GetRawAxis(3); //HEY magic numebr again should be right trigger
	if(manualIntakeThrottle >= 0.5){CollectorMotor(MANUAL_SPIT_SPEED);}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Manual Rollers Intake (Slow & Fast)~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//if right bumper pressed fast, if back and right bumper pressed go slower? idk why we need this it seems dumb -Ben L.
	if(Robot::m_oi->OperatorGamepad()->GetRawButton(6)) CollectorMotor(MANUAL_INTAKE_FAST);
	else if (Robot::m_oi->OperatorGamepad()->GetRawButton(6) && Robot::m_oi->OperatorGamepad()->GetRawButton(7)) CollectorMotor(MANUAL_INTAKE_SLOW);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Manual Collector Control~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	double yR = Robot::m_oi->OperatorGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_Y);
	if(fabs(yR) <= DEADBAND_CONST) yR = 0;	//Deadband code
	//if joystick pushed up, cage goes up
	//if joystick pushed down, cage goes down
	if(yR >= 0.5) RetractCage();
	if(yR <= -0.5) DeployCage();
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
//********COLLECTOR MOTOR***********
void Collector::CollectorMotor(double power)
{
	collectorMotor->Set(power);
}
void Collector::StopCollectorMotor(void)
{
	collectorMotor->Set(0.0);
}