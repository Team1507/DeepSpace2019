
#include "commands/CmdSetStilts.h"

#include "Robot.h"

CmdSetStilts::CmdSetStilts(int position) {
    m_position = position;
    // Use Requires() here to declare subsystem dependencies
    Requires(Robot::m_drivetrain);
}

// Called just before this Command runs the first time
void CmdSetStilts::Initialize() {
    std::cout<<"CmdSetStilts" << std::endl;
    Robot::m_drivetrain->SetStilts(m_position);
}

// Called repeatedly when this Command is scheduled to run
void CmdSetStilts::Execute() {}

// Make this return true when this Command no longer needs to run execute()
bool CmdSetStilts::IsFinished() { 
    return true; 
    }

// Called once after isFinished returns true
void CmdSetStilts::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdSetStilts::Interrupted() {}
