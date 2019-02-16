
#include "commands/CmdDriveManual.h"
#include "subsystems/Drivetrain.h"
#include "Robot.h"

CmdDriveManual::CmdDriveManual(double left_drive, double right_drive, double time) 
{
    m_left_drive = left_drive;
    m_right_drive = right_drive;
    m_time = time;
   
    Requires(Robot::m_drivetrain);
    
}

void CmdDriveManual::Initialize() {
    Robot::m_drivetrain->Drive( m_left_drive,  m_right_drive );
	SetTimeout (m_time);
}


void CmdDriveManual::Execute() {}


bool CmdDriveManual::IsFinished() { 
     return IsTimedOut();
 }

// Called once after isFinished returns true
void CmdDriveManual::End() {
    Robot::m_drivetrain->Stop();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdDriveManual::Interrupted() 
{
    End();
}
