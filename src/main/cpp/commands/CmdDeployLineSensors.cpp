
#include "commands/CmdDeployLineSensors.h"

CmdDeployLineSensors::CmdDeployLineSensors(bool value)
{
  m_value = value;
}

// Called once when the command executes
void CmdDeployLineSensors::Initialize() 
{
  if(m_value)
    Robot::m_drivetrain->LineSensorsDeploy();
  else
    Robot::m_drivetrain->LineSensorsRetract();
}
