

#include "commands/CmdElevatorGo2RocketMid.h"
#include "commands/CmdPrintAutoText.h"

CmdElevatorGo2RocketMid::CmdElevatorGo2RocketMid()
{
 
}

// Called once when the command executes
void CmdElevatorGo2RocketMid::Initialize() 
{
    Robot::m_elevator->SetReqElevatorValue(Elevator::ROCKET_VALUE);  //was 
   
}
void CmdElevatorGo2RocketMid::Execute() {

}

bool CmdElevatorGo2RocketMid::IsFinished() {
    return true;
}
void CmdElevatorGo2RocketMid::End(){
   
}
void CmdElevatorGo2RocketMid::Interrupted(){
    
}