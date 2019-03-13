

#include "Commands\CmdBuzzGamepad.h"
#include "Robot.h"

CmdBuzzGamepad::CmdBuzzGamepad()
{

}

// Called just before this Command runs the first time
void CmdBuzzGamepad::Initialize() 
{
		SetTimeout (0.5);
		Robot::m_driverfeedback->RumbleOn();
}

// Called repeatedly when this Command is scheduled to run
void CmdBuzzGamepad::Execute() 
{
}

// Make this return true when this Command no longer needs to run execute()
bool CmdBuzzGamepad::IsFinished() 
{

	if ( IsTimedOut() )
	{
		return true;
	}

	return false;
}

// Called once after isFinished returns true
void CmdBuzzGamepad::End() 
{
		Robot::m_driverfeedback->RumbleOff();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdBuzzGamepad::Interrupted() 
{
  End();
}
