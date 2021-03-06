#include "commands/CmdConeControl.h"
#include "Robot.h"
CmdConeControl::CmdConeControl(bool makeConeUp)
{
    m_coneUp = makeConeUp;
    Requires( Robot::m_carriage);
}

void CmdConeControl::Initialize() 
{
    if(m_coneUp)Robot::m_carriage->TiltUp();
    else Robot::m_carriage->TiltDown();
}

void CmdConeControl::Execute() {}

bool CmdConeControl::IsFinished() { return true; }

void CmdConeControl::End() {}

void CmdConeControl::Interrupted() {}
