

#include "Commands/CmdDriveRevTurn2Heading.h"
#include "Robot.h"

CmdDriveRevTurn2Heading::CmdDriveRevTurn2Heading(double power, double heading) 
{
    m_power   = power;
    m_heading = heading;
	
    // Use Requires() here to declare subsystem dependencies
    // eg. Requires(Robot::chassis.get());
    Requires( Robot::m_drivetrain);

}

// Called just before this Command runs the first time
void CmdDriveRevTurn2Heading::Initialize() 
{
    
	m_isRightTurn = Robot::m_drivetrain->GetGyroAngle() < m_heading; //was >
	//m_heading = (Robot::m_drivetrain->GetGyroYaw() - m_heading) * -.90; //this is Mr. B's compensating for drift //was -.92
    if( m_isRightTurn)  std::cout<< "RT2H: Right Turn" << std::endl;
    else                std::cout<< "RT2H: Left Turn" << std::endl;
}

// Called repeatedly when this Command is scheduled to run
void CmdDriveRevTurn2Heading::Execute() 
{
	if( m_isRightTurn )
	{
		//Reverse Right turn.  Drive Right motor hard
		Robot::m_drivetrain->Drive(0.0, m_power*(-1.0)); //"no magic numbers" said kris; was .1
	}
	else
	{
		//Reverse Left turn.  Drive Left motor hard
		Robot::m_drivetrain->Drive(m_power*(-1.0), 0.0); //"no magic numbers" said kris; was .1
	}

}

// Make this return true when this Command no longer needs to run execute()
bool CmdDriveRevTurn2Heading::IsFinished()
{ 
	if( m_isRightTurn )
	{
		//Reverse Right turn: Done when Yaw becomes greater than target heading 
		if(Robot::m_drivetrain->GetGyroAngle() >= m_heading)
		{
			return true;
		}
		
	}
	else
	{
		//Reverse Left Turn:  Done when Yaw becomes less than target heading
		if(Robot::m_drivetrain->GetGyroAngle() <= m_heading)
		{
			return true;
		}
		
	}
	return false;
}

// Called once after isFinished returns true
void CmdDriveRevTurn2Heading::End() 
{
	std::cout<<"GyroAngle ";
	std::cout<<Robot::m_drivetrain->GetGyroAngle()<<std::endl;
	std::cout<<"EncoderValueL ";
	std::cout<<Robot::m_drivetrain->GetLeftEncoder()<<std::endl;
	std::cout<<"EncoderValueR ";
	std::cout<<Robot::m_drivetrain->GetRightEncoder()<<std::endl;
    Robot::m_drivetrain->Stop();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdDriveRevTurn2Heading::Interrupted() {}
