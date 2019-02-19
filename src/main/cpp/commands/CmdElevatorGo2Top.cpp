
#include "commands/CmdElevatorGo2Top.h"
#include "commands/CmdPrintAutoText.h"

CmdElevatorGo2Top::CmdElevatorGo2Top()
{
 
}

// Called once when the command executes
void CmdElevatorGo2Top::Initialize() 
{
    Robot::m_elevator->SetReqElevatorValue(Elevator::ROCKET_HIGH_VALUE);  //was 
   
}
void CmdElevatorGo2Top::Execute() {

}

bool CmdElevatorGo2Top::IsFinished() {
    return true;
}
void CmdElevatorGo2Top::End(){
   
}
void CmdElevatorGo2Top::Interrupted(){
    
}