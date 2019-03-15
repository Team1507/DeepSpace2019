

#include "Commands\CmdDriveClearEncoderV2.h"
#include "Robot.h"

CmdDriveClearEncoderV2::CmdDriveClearEncoderV2()
{
m_left=0;
m_right=0;
}

// Called just before this Command runs the first time
void CmdDriveClearEncoderV2::Initialize() 
{
	m_left  = Robot::m_drivetrain->GetLeftEncoder();
	m_right = Robot::m_drivetrain->GetRightEncoder();
    Robot::m_drivetrain->ResetEncoders();
}

// Called repeatedly when this Command is scheduled to run
void CmdDriveClearEncoderV2::Execute() 
{
	Robot::m_drivetrain->ResetEncoders();
}

// Make this return true when this Command no longer needs to run execute()
bool CmdDriveClearEncoderV2::IsFinished() 
{

	if ( (Robot::m_drivetrain->GetLeftEncoder() < m_left ) && (Robot::m_drivetrain->GetRightEncoder() < m_right )   )
	{
		return true;
	}

	return false;
}

// Called once after isFinished returns true
void CmdDriveClearEncoderV2::End() 
{
	std::cout<< "EncoderClearV2 = " << Robot::m_drivetrain->GetLeftEncoder() << " " << Robot::m_drivetrain->GetRightEncoder() << "!!!" <<std::endl;
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdDriveClearEncoderV2::Interrupted() 
{
  End();
}
