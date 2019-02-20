#include "commands/CmdHatchEject.h"
#include "Robot.h"
#include "subsystems/Carriage.h"

CmdHatchEject::CmdHatchEject() {
  Requires(Robot::m_carriage);
}

// Called just before this Command runs the first time
void CmdHatchEject::Initialize() 
{
  Robot::m_carriage->SetLatch(Carriage::LATCH_CLOSE);
}

void CmdHatchEject::Execute() {}

bool CmdHatchEject::IsFinished() { return true; }

void CmdHatchEject::End() {}

void CmdHatchEject::Interrupted() {}
