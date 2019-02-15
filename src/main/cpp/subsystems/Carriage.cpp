#include "subsystems/Carriage.h"
#include "Robot.h"

const int Carriage::LATCH_CLOSE = 0;
const int Carriage::LATCH_OPEN = 1;
const int Carriage::TILT_DOWN = 0;
const int Carriage::TILT_UP = 1;
const int Carriage::BRIDGE_SPEED = 0.2; //why is this an int?
const double Carriage::GETTING_BALL_SPEED = .3;	//may want to change
const double Carriage::SHOOTING_BALL_SPEED = .7;//may want to change

Carriage::Carriage() : Subsystem("Carriage") {
  carriageHatchLatch = new frc::DoubleSolenoid(2,4,5);
  carriageTilt       = new frc::DoubleSolenoid(2,2,3);
  carriagePhotoeye   = new frc::DigitalInput(5);
  carriageRollers    = new frc::Spark(1);
}

void Carriage::InitDefaultCommand() {}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~PERIODIC START~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Carriage::Periodic()
{
	//******************Hatch grab and release*****************
	if(Robot::m_oi->OperatorGamepad()->GetRawButton(3)) OpenLatch();
	else CloseLatch();

	//******************Carrige Tilt***************************
	static bool tiltFlag = false;
	bool yPressed = Robot::m_oi->OperatorGamepad()->GetRawButton(4); //HEY its a magic number again suprise
	if( !(IsCarriageTiltedUp()) && yPressed && !(tiltFlag))
	{
		tiltFlag = true;
		TiltDown();
	}
	else if( IsCarriageTiltedUp() && yPressed && !(tiltFlag))
	{
		tiltFlag = true;
		TiltUp();
	}
	else
	{
		tiltFlag = false;
	}

	//*************Carrige Rollers Intake / Out / OutFast***********
	//if left trigger pushed get ball from human, if left bumper pushed shoot, if left bumper and back pushed, accept from bridge
	if( Robot::m_oi->OperatorGamepad()->GetRawAxis(2) >= .5 )CarriageRollers(GETTING_BALL_SPEED);
	if( Robot::m_oi->OperatorGamepad()->GetRawButton(5) )CarriageRollers(SHOOTING_BALL_SPEED);
	else if(  Robot::m_oi->OperatorGamepad()->GetRawButton(5) && Robot::m_oi->OperatorGamepad()->GetRawButton(7)  )CarriageRollers(GETTING_BALL_SPEED);
}


//********PhotoEye***********
bool Carriage::IsPhotoeyeDetected(void)
{
	return carriagePhotoeye->Get();
}

//********HATCH LATCH***********
void Carriage::OpenLatch(void)
{
	carriageHatchLatch->Set(DoubleSolenoid::kForward);
	m_OpenLatch = true;
}
void Carriage::CloseLatch(void)
{
	carriageHatchLatch->Set(DoubleSolenoid::kReverse);
	m_OpenLatch = false;
	Robot::m_driverfeedback->RumbleOn();
}
void Carriage::SetLatch(int position)
{
	if(position == LATCH_OPEN )  OpenLatch();
	if(position == LATCH_CLOSE)  CloseLatch();
}
bool Carriage::IsHatchLatched(void){
	return m_OpenLatch;
}

//********TILT***********
void Carriage::TiltUp(void)
{
	carriageHatchLatch->Set(DoubleSolenoid::kForward);
	m_TiltUp = true;
}
void Carriage::TiltDown(void)
{
	carriageHatchLatch->Set(DoubleSolenoid::kReverse);
	m_TiltUp = false;
}
void Carriage::SetTilt(int position)
{
	if(position == TILT_UP )   TiltUp();
	if(position == TILT_DOWN)  TiltDown();
}
bool Carriage::IsCarriageTiltedUp(void){
	return m_TiltUp;
}

//********ROLLERS***********
void Carriage::CarriageRollers(double power)
{
	carriageRollers->Set(power);
}
void Carriage::StopCarriageRollers(void)
{
	carriageRollers->Set(0.0);
}