#include "subsystems/Collector.h"
#include "Robot.h"

const int Collector::RETRACT_CAGE = 0;
const int Collector::DEPLOY_CAGE = 1;
const int Collector::CLOSE_BRIDGE = 0;
const int Collector::OPEN_BRIDGE = 1;
const int Collector::COLLECT_SPEED = 0.3;
const int Collector::BRIDGE_SPEED = 0.2;

Collector::Collector() : Subsystem("Collector")
{ 
    collectorCage = new frc::DoubleSolenoid(1,2,3);
	collectorBridge = new frc::DoubleSolenoid(1,6,7);
    collectorPhotoeye = new frc::DigitalInput(4);
    collectorMotor =  new frc::Spark(5); //should be 0
    //m_CageDeploy = false;
}

void Collector::InitDefaultCommand() {}

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