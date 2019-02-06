

#include "Commands\CmdGetCargoFromFloor.h"
#include "Robot.h"
#include "subsystems/Elevator.h"
#include "subsystems/Collector.h"
#include "subsystems/Carriage.h"

#define STAGE1 1
#define STAGE2 2
#define STAGE3 3
#define STAGE4 4
#define STAGE5 5
#define STAGE6 6
#define STAGE7 7
#define STAGE8 8
#define STAGE_DELAY  9

#define ONE_TO_TWO_DELAY 50

CmdGetCargoFromFloor::CmdGetCargoFromFloor()
{   
}

// Called just before this Command runs the first time
void CmdGetCargoFromFloor::Initialize() 
{
   m_state = STAGE1;
   m_delayIterations = 0;
}

// Called repeatedly when this Command is scheduled to run
void CmdGetCargoFromFloor::Execute() 
{
    switch(m_state)
	{
		case STAGE1://Initial Subsystem positions
			Robot::m_elevator->SetReqElevatorValue(Elevator::BOT_VALUE);//Lower Elevator
            Robot::m_collector->SetCage(Collector::DEPLOY_CAGE);//Drop collector
            Robot::m_carriage->SetTilt(Carriage::TILT_DOWN);//tilt pointy thing in the front down
            Robot::m_collector->SetBridge(Collector::CLOSE_BRIDGE);//Close bridge
            std::cout<<"STAGE1 Done"<<std::endl;
            m_state = STAGE_DELAY;              //go to delay
            m_nextState = STAGE2;               //Setting the stage to go to after delay
            m_delayIterations = ONE_TO_TWO_DELAY; //Setting the delay length
            break;

        case STAGE2://Disable Joystick Controls (we don't know how)
            //Requires subsystem
            std::cout<<"STAGE2 Done"<<std::endl;
            m_state = STAGE3;
            break;

        case STAGE3://Grab ball
            //Robot::m_collector->CollectorMotor(Collector::COLLECT_SPEED); Ben L said this looked unnecessary
            if(Robot::m_collector->IsPhotoeyeDetected())//stops when detected
            {
		        Robot::m_collector->StopCollectorMotor();
                std::cout<<"STAGE3 Done"<<std::endl;
                m_state = STAGE4;
            }
            else
            {
                Robot::m_collector->CollectorMotor(Collector::COLLECT_SPEED); //Starts collector motor
            }
            break;

        case STAGE4://Raise Collector Cage
            Robot::m_collector->RetractCage();//Retract cage
            std::cout<<"STAGE4 Done"<<std::endl;
            m_state = STAGE5;
            break;

        case STAGE5://Open bridge
            Robot::m_collector->SetBridge(Collector::OPEN_BRIDGE); //Open the bridge
            std::cout<<"STAGE5 Done"<<std::endl;
            m_nextState = STAGE6; //Set next state for delay's completion
            m_state = STAGE_DELAY; //go to delay
            break;

        case STAGE6://transfer cargo to the scorer
            Robot::m_carriage->CarriageRollers(Carriage::BRIDGE_SPEED);//start scorer
            Robot::m_collector->CollectorMotor(Collector::BRIDGE_SPEED);//start collector motor transfer
            std::cout<<"STAGE6 Done"<<std::endl;
            m_state = STAGE7;
            break;

        case STAGE7://Close bridge
            Robot::m_collector->SetBridge(Collector::CLOSE_BRIDGE);//Close bridge
            std::cout<<"STAGE7 Done"<<std::endl;
            m_state = STAGE8;
            break;

        case STAGE8://Restore manual controls
            std::cout<<"STAGE8 Done"<<std::endl;
            m_state = STAGE7;
            break;


        case STAGE_DELAY: //This is our multi purpose delay "timer"
            if(m_delayIterations > 0)
            {
                m_delayIterations--;
            }
            else
            {
                m_state = m_nextState;   
                std::cout<<"Timer Done"<<std::endl;
            }
            break;
    }
}

// Make this return true when this Command no longer needs to run execute()
bool CmdGetCargoFromFloor::IsFinished() 
{
	

	return false;
}

// Called once after isFinished returns true
void CmdGetCargoFromFloor::End() 
{
 
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdGetCargoFromFloor::Interrupted() 
{
  End();
}



//ligma