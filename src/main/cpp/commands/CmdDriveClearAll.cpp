
#include "Commands/CmdDriveClearAll.h"
#include "Robot.h"

CmdDriveClearAll::CmdDriveClearAll() {
  // Use Requires() here to declare subsystem dependencies
  // eg. Requires(Robot::chassis.get());
}

// Called once when the command executes
void CmdDriveClearAll::Initialize() 
{
    //Use at the start of Auto to clear/reset all drivetrain parameters
    Robot::m_drivetrain->ResetEncoders();
    Robot::m_drivetrain->ZeroGyro();

}
