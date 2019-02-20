#include "subsystems/Collector.h"
#include "subsystems/Drivetrain.h"
#include "subsystems/Carriage.h"
#include "Robot.h"
#include "OI.h"
#include "GamepadMap.h" 

#define DEADBAND_CONST  .08

const int Collector::RETRACT_CAGE = 0;
const int Collector::DEPLOY_CAGE = 1;
const int Collector::CLOSE_BRIDGE = 0;
const int Collector::OPEN_BRIDGE = 1;
const double Collector::COLLECT_SPEED = 0.3;
const double Collector::BRIDGE_SPEED = 0.4;

const double Collector::MANUAL_SPIT_SPEED = -1.0;
const double Collector::MANUAL_INTAKE_FAST = 0.7;  //May want to change 
const double Collector::MANUAL_INTAKE_SLOW = 0.4;
bool lastStatePhotoeye = false;
Collector::Collector() : Subsystem("Collector")
{ 
    collectorCage = new frc::DoubleSolenoid(1,2,3);
	collectorBridge = new frc::DoubleSolenoid(1,6,7);
    collectorPhotoeye = new frc::DigitalInput(7);
    collectorRollers =  new VictorSPX(14); 
    m_autocollect = false;
	m_autoXfer = false;
	m_DeployCage = false;
	
}

void Collector::InitDefaultCommand() {}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~PERIODIC START~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
	else if (!m_autocollect && !m_autoXfer) //if we aren't using auto collect and transfer then stop stop spinning
	{
		CollectorRollers(0.0);
	}
	//************Auto Collector Control**************
	bool A_Pressed = Robot::m_oi->OperatorGamepad()->GetRawButtonPressed(GAMEPADMAP_BUTTON_A);
	if (IsCageDeployed() && A_Pressed && (!m_autocollect)) //Start rollers
	{
		m_autocollect = true;
		CollectorRollers(MANUAL_INTAKE_FAST);
	}
	else if (A_Pressed && m_autocollect) //Abort auto collection
	{
		CollectorRollers(0.0);
		m_autocollect = false;
	}
	else if (m_autocollect && IsCollectorPhotoeyeDetected()) //Bring collector up if we have a ball
	{
		CollectorRollers(0.0);
		RetractCage();
		m_autocollect = false;
	}
	//~~~~~~~~~~~~~~~~~~~~Manual Collector Control~~~~~~~~~~~~~~~~~~~~~
	
	double yR = Robot::m_oi->OperatorGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_Y);
	if(fabs(yR) <= DEADBAND_CONST) yR = 0;	//Deadband code
	if(yR >= 0.5) DeployCage();//if joystick pushed up, cage goes up
	if(yR <= -0.5) 
	{
		CollectorRollers(0.0);
		RetractCage();
		m_autocollect = false; //if joystick pushed down, cage goes down
	}
	//************Auto Ball Transfer*****************
	
	if(Robot::m_oi->OperatorGamepad()->GetRawButtonPressed(GAMEPADMAP_BUTTON_B))
	{
		m_autoXfer = true;
		OpenBridge();
		CollectorRollers(0.4);
		Robot::m_carriage->CarriageRollers(0.4);
	}
	else if(Robot::m_carriage->IsFrontPhotoeyeDetected() && m_autoXfer)
	{
		Robot::m_carriage->CarriageRollers(0.0); //mr b said this should go first and I agree benl. :))
		m_autoXfer = false;
		CloseBridge();
		CollectorRollers(0.0);
	}//HEY ask mr sielski if we should need an abort if the photoeye is broke
}

//********************************************END OF PERIODIC**********************************************************


//********PhotoEye***********
bool Collector::IsCollectorPhotoeyeDetected(void)
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