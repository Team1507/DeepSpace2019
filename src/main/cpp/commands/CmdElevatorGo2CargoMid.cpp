

#include "commands/CmdElevatorGo2CargoMid.h"
#include "commands/CmdPrintAutoText.h"

CmdElevatorGo2CargoMid::CmdElevatorGo2CargoMid()
{
 
}

// Called once when the command executes
void CmdElevatorGo2CargoMid::Initialize() 
{
    Robot::m_elevator->SetReqElevatorValue(Elevator::CARGO_VALUE);  //was 
   
}
void CmdElevatorGo2CargoMid::Execute() {

}

bool CmdElevatorGo2CargoMid::IsFinished() {
    return true;
}
void CmdElevatorGo2CargoMid::End(){
   
}
void CmdElevatorGo2CargoMid::Interrupted(){
    
}