

#include "Commands/CmdDriveTurn2Heading.h"
#include "Robot.h"

CmdDriveTurn2Heading::CmdDriveTurn2Heading(double leftPower, double rightPower, double heading, double timeout) 
{
    m_leftPower   = leftPower;
    m_rightPower   = rightPower;
    m_heading = heading;
	m_timeout  = timeout;
	
    // Use Requires() here to declare subsystem dependencies
    // eg. Requires(Robot::chassis.get());
    Requires( Robot::m_drivetrain);

}

// Called just before this Command runs the first time
void CmdDriveTurn2Heading::Initialize() 
{
    
	m_isRightTurn = Robot::m_drivetrain->GetGyroAngle() < m_heading;
	//m_heading = (Robot::m_drivetrain->GetGyroYaw() - m_heading) * -.90; //this is Mr. B's compensating for drift //was -.92
    if( m_isRightTurn)  std::cout<< "T2H: Right Turn" << std::endl;
    else                std::cout<< "T2H: Left Turn" << std::endl;

	if( m_timeout > 0.0)
	{
		SetTimeout (m_timeout);
	}	
}

// Called repeatedly when this Command is scheduled to run
void CmdDriveTurn2Heading::Execute() 
{
	// if( m_isRightTurn )
	// {
	// 	//Right turn.  Drive LEFT motor hard
	// 	Robot::m_drivetrain->Drive(m_power, -0.35); //"no magic numbers" said kris; was .3
	// }
	// else
	// {
	// 	//Left turn.  Drive RIGHT motor hard
	// 	Robot::m_drivetrain->Drive(-0.35, m_power); //"no magic numbers" said kris; was .3
	// }
	Robot::m_drivetrain->Drive(m_leftPower, m_rightPower); //"no magic numbers" said kris; was .3
}

// Make this return true when this Command no longer needs to run execute()
bool CmdDriveTurn2Heading::IsFinished()
{ 
	if( m_isRightTurn )
	{
		//Right turn: Done when Yaw becomes greater than calc angle
		if(Robot::m_drivetrain->GetGyroAngle() >= m_heading)
		{
			return true;
		}
		
	}
	else
	{
		//Left Turn:  Done when Yaw becomes less than calc angle
		if(Robot::m_drivetrain->GetGyroAngle() <= m_heading)
		{
			return true;
		}
		
	}
	if ((m_timeout>0.0) && IsTimedOut())
	{
		std::cout<<"CmdDriveTurn2Heading: Timeout"<<std::endl;
		return true;
	}	
	return false;
}

// Called once after isFinished returns true
void CmdDriveTurn2Heading::End() 
{
	std::cout<<"GyroAngle ";
	std::cout<<Robot::m_drivetrain->GetGyroAngle()<<std::endl;
	std::cout<<"EncoderValueL ";
	std::cout<<Robot::m_drivetrain->GetLeftEncoder()<<std::endl;
	std::cout<<"EncoderValueR ";
	std::cout<<Robot::m_drivetrain->GetRightEncoder()<<std::endl;
    Robot::m_drivetrain->Stop();

	Robot::m_drivetrain->ResetEncoders();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdDriveTurn2Heading::Interrupted() {}
