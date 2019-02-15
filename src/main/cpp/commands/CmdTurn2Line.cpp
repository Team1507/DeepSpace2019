#include "commands/CmdTurn2Line.h"
#include "Robot.h"
#include "subsystems/drivetrain.h"

CmdTurn2Line::CmdTurn2Line(bool isLeftTurn, double throttle) {
   
    m_isLeftTurn = isLeftTurn;
    m_throttle = throttle;
    
    Requires(Robot::m_drivetrain);
  
  
}

void CmdTurn2Line::Initialize() {
    std::cout<<"CmdTurn2Line" << std::endl;
    if(m_isLeftTurn) Robot::m_drivetrain->Drive(m_throttle *(-1.0), m_throttle); //drives the right side positive
    else Robot::m_drivetrain->Drive(m_throttle, m_throttle*(-1.0)); //drives the left side positive


    
}

void CmdTurn2Line::Execute() {}


bool CmdTurn2Line::IsFinished() { 
    if(m_isLeftTurn)
    {
        if(Robot::m_drivetrain->lineStateReturn() == 100){return true;}
        else {return false;}
    }
    else
    {
        if(Robot::m_drivetrain->lineStateReturn() == 1){return true;}
        else{return false;}
    }
}

void CmdTurn2Line::End() {
    Robot::m_drivetrain->Drive(0,0); 
}


void CmdTurn2Line::Interrupted() {}
