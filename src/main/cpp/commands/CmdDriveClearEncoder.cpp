
#include "Commands/CmdDriveClearEncoder.h"
#include "Robot.h"

CmdDriveClearEncoder::CmdDriveClearEncoder() {
  // Use Requires() here to declare subsystem dependencies
  // eg. Requires(Robot::chassis.get());
}

// Called once when the command executes
void CmdDriveClearEncoder::Initialize() 
{
    //Use at the start of Auto to clear/reset all drivetrain parameters
    Robot::m_drivetrain->ResetEncoders();
}
