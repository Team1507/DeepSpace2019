#include "Robot.h"
#include <iostream>
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include "subsystems/DriverFeedback.h"
#include <frc/GenericHID.h>

#define FULL_RUMBLE 1.0

DriverFeedback::DriverFeedback() : Subsystem("DriverFeedback") {
    l_canifier = new CANifier(5);
    r_canifier = new CANifier(6);

/* Instantiate objects */
	//static CANifier *l_canifier;
    //static CANifier *r_canifier;
}

void DriverFeedback::InitDefaultCommand() {
  // Set the default command for a subsystem here.
  // SetDefaultCommand(new MySpecialCommand());
	
}

// Put methods for controlling this subsystem
// here. Call these from Commands.
void DriverFeedback::DriverFeedbackPeriodic()
{
   	 
}

void DriverFeedback::UpdateLeftLEDs(float _r, float _g, float _b){
    
	/* Update CANifier's LED strip */
	//Hardware::
	//Channel A and B is incorrect in documentation
	 
    l_canifier->SetLEDOutput(_g, CANifier::LEDChannel::LEDChannelA);
	l_canifier->SetLEDOutput(_r, CANifier::LEDChannel::LEDChannelB);
	l_canifier->SetLEDOutput(_b, CANifier::LEDChannel::LEDChannelC);
    
}

void DriverFeedback::UpdateRightLEDs(float _r, float _g, float _b){
   
	/* Update CANifier's LED strip */
	//Hardware::
	//Channel A and B is incorrect in documentation 
   

    r_canifier->SetLEDOutput(_g, CANifier::LEDChannel::LEDChannelA);
	r_canifier->SetLEDOutput(_r, CANifier::LEDChannel::LEDChannelB);
	r_canifier->SetLEDOutput(_b, CANifier::LEDChannel::LEDChannelC);    
    
}

void DriverFeedback::LeftLEDsOff(){

    l_canifier->SetLEDOutput(0, CANifier::LEDChannel::LEDChannelA);
	l_canifier->SetLEDOutput(0, CANifier::LEDChannel::LEDChannelB);
	l_canifier->SetLEDOutput(0, CANifier::LEDChannel::LEDChannelC);

}
void DriverFeedback::RightLEDsOff(){

    r_canifier->SetLEDOutput(0, CANifier::LEDChannel::LEDChannelA);
	r_canifier->SetLEDOutput(0, CANifier::LEDChannel::LEDChannelB);
	r_canifier->SetLEDOutput(0, CANifier::LEDChannel::LEDChannelC);  

}
void DriverFeedback::RumbleOn(void){
	//std::shared_ptr<frc::Joystick> gamepad = Robot::oi->getDriverGamepad();


	Robot::m_oi->DriverGamepad()->SetRumble(frc::GenericHID::kLeftRumble, 0.4);
	Robot::m_oi->DriverGamepad()->SetRumble(frc::GenericHID::kRightRumble, 0.4);

	Robot::m_oi->OperatorGamepad()->SetRumble(frc::GenericHID::kLeftRumble, 0.4);
	Robot::m_oi->OperatorGamepad()->SetRumble(frc::GenericHID::kRightRumble, 0.4);

}
void DriverFeedback::RumbleOff(void){
	Robot::m_oi->DriverGamepad()->SetRumble(frc::GenericHID::kLeftRumble, 0.0);
	Robot::m_oi->DriverGamepad()->SetRumble(frc::GenericHID::kRightRumble, 0.0);

	Robot::m_oi->OperatorGamepad()->SetRumble(frc::GenericHID::kLeftRumble, 0.0);
	Robot::m_oi->OperatorGamepad()->SetRumble(frc::GenericHID::kRightRumble, 0.0);
}
