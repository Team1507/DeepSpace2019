

#include "commands/CmdElevatorGo2Bot.h"
#include "commands/CmdPrintAutoText.h"

CmdElevatorGo2Bot::CmdElevatorGo2Bot()
{
 
}

// Called once when the command executes
void CmdElevatorGo2Bot::Initialize() 
{
    Robot::m_elevator->SetReqElevatorValue(Elevator::BOT_VALUE);  //was 
   
}
void CmdElevatorGo2Bot::Execute() {

}

bool CmdElevatorGo2Bot::IsFinished() {
    return true;
}
void CmdElevatorGo2Bot::End(){
   
}
void CmdElevatorGo2Bot::Interrupted(){
    
}