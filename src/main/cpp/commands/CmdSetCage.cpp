

#include "commands/CmdSetCage.h"

#include "Robot.h"

CmdSetCage::CmdSetCage(int position) {
    m_position = position;
    // Use Requires() here to declare subsystem dependencies
    Requires(Robot::m_collector);
}

// Called just before this Command runs the first time
void CmdSetCage::Initialize() 
{
    std::cout<<"CmdSetCage" << std::endl;
    Robot::m_collector->SetCage(m_position);
}

// Called repeatedly when this Command is scheduled to run
void CmdSetCage::Execute() 
{
}

// Make this return true when this Command no longer needs to run execute()
bool CmdSetCage::IsFinished() 
{ 
    return true;
}

// Called once after isFinished returns true
void CmdSetCage::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdSetCage::Interrupted() {}
