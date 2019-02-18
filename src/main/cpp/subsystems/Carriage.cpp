#include "subsystems/Carriage.h"
#include "Robot.h"
#include "GamepadMap.h"
#include "subsystems/DriverFeedback.h"
#include "subsystems/Collector.h"
const int Carriage::LATCH_CLOSE = 0;
const int Carriage::LATCH_OPEN = 1;
const int Carriage::TILT_DOWN = 0;
const int Carriage::TILT_UP = 1;
const double Carriage::BRIDGE_SPEED = 0.4; 
const double Carriage::GETTING_BALL_SPEED = 0.5;	//may want to change
const double Carriage::SHOOTING_BALL_SPEED = 1.0;  //may want to change

Carriage::Carriage() : Subsystem("Carriage") {
  	carriageHatchLatch = new frc::DoubleSolenoid(2,4,5);
  	carriageTilt       = new frc::DoubleSolenoid(2,2,3);
  	frontcarriagePhotoeye  = new frc::DigitalInput(9);
	rearcarriagePhotoeye   = new frc::DigitalInput(8);
 	carriageRollers    = new VictorSPX(13);
}

void Carriage::InitDefaultCommand() {}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~PERIODIC START~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Carriage::CarriagePeriodic()
{
	//******************Hatch grab and release*****************
	static unsigned char rumbleLatchFlag = 25;
	static unsigned char rumbleReleaseFlag = 25;
	if(Robot::m_oi->OperatorGamepad()->GetRawButton(GAMEPADMAP_BUTTON_X))//While the button is held
	{
		if(Robot::m_oi->OperatorGamepad()->GetRawButtonPressed(GAMEPADMAP_BUTTON_X))//Rumble on and close latch ONCE
		{
			Robot::m_driverfeedback->RumbleOn();
			CloseLatch();
			rumbleReleaseFlag = 25;	//Reset second timer
			Robot::m_driverfeedback->UpdateLeftLEDs(PURPLE_rgb);
			Robot::m_driverfeedback->UpdateRightLEDs(PURPLE_rgb);
		}
		//Start counting down, when done stop rumbling
		if(rumbleLatchFlag > 0) rumbleLatchFlag--;
		if(rumbleLatchFlag == 1) Robot::m_driverfeedback->RumbleOff();
	}
	else
	{
		if((Robot::m_oi->OperatorGamepad()->GetRawButtonReleased(GAMEPADMAP_BUTTON_X))) // if the button is released
		{
			Robot::m_driverfeedback->RumbleOn();
			OpenLatch();
			rumbleLatchFlag = 25; //Reset 1st Timer
			Robot::m_driverfeedback->LeftLEDsOff();
			Robot::m_driverfeedback->RightLEDsOff();
		}
		//Start counting down, when done stop rumbling
		if(rumbleReleaseFlag > 0) rumbleReleaseFlag--;
		if(rumbleReleaseFlag == 1) Robot::m_driverfeedback->RumbleOff();
	}
	
	//******************Carrige Tilt***************************
	bool yPressed = Robot::m_oi->OperatorGamepad()->GetRawButtonPressed(GAMEPADMAP_BUTTON_Y);

	if(yPressed)
	{
		if(IsCarriageTiltedUp())
			TiltDown();
		else
			TiltUp();
	}

	//*****************Carrige Rollers Intake / Out / OutFast***************
	if(!Robot::m_collector->m_autoXfer)
	{ //if left trigger pushed get ball from human
		if( Robot::m_oi->OperatorGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_TRIG) >= .5 )
		{
			CarriageRollers(GETTING_BALL_SPEED);
		}
	//if left bumper pushed shoot, if left bumper and back pushed, accept from bridge
		else if( Robot::m_oi->OperatorGamepad()->GetRawButtonPressed(GAMEPADMAP_BUTTON_LBUMP) )
		{
			CarriageRollers(SHOOTING_BALL_SPEED);
		}
	// else if(  Robot::m_oi->OperatorGamepad()->GetRawButton(GAMEPADMAP_BUTTON_LBUMP) && Robot::m_oi->OperatorGamepad()->GetRawButton(GAMEPADMAP_BUTTON_BACK) )
	// {
	// 	CarriageRollers(GETTING_BALL_SPEED);
	// }
	else if( Robot::m_oi->OperatorGamepad()->GetRawButtonReleased(GAMEPADMAP_BUTTON_LBUMP) )
		{
			CarriageRollers(0.0);
		}
		
	
	}
}


//********Front PhotoEye***********
bool Carriage::IsFrontPhotoeyeDetected(void)
{
	return frontcarriagePhotoeye->Get();
}

//********Rear PhotoEye***********
bool Carriage::IsRearPhotoeyeDetected(void)
{
	return rearcarriagePhotoeye->Get();
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
	carriageTilt->Set(DoubleSolenoid::kForward);
	m_TiltUp = true;
}
void Carriage::TiltDown(void)
{
	carriageTilt->Set(DoubleSolenoid::kReverse);
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
	carriageRollers->Set(ControlMode::PercentOutput, power);
}
void Carriage::StopCarriageRollers(void)
{
	carriageRollers->Set(ControlMode::PercentOutput, 0.0);
}

// Victor Init of Rollers
void Carriage::VictorSPXInit(void)
{
	carriageRollers->ConfigFactoryDefault();

	carriageRollers->SetInverted(true);

	carriageRollers->SetNeutralMode(NeutralMode::Brake);

}