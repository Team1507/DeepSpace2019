

#include "Commands\CmdDriveRevGyroPitchDown.h"
#include "Robot.h"

const double PITCH_LIMIT = -15.0;

CmdDriveRevGyroPitchDown::CmdDriveRevGyroPitchDown(double power, double heading, double distance, bool stop, double timeout)
{
  m_power    = (-1.0) * power;		//Reverse = negative power
	m_heading  = heading;
  m_distance = (-1.0) * distance;	//Reverse = Negative distance
  m_stop     = stop;
  m_timeout  = timeout;

  // Use Requires() here to declare subsystem dependencies
  Requires( Robot::m_drivetrain);
}

// Called just before this Command runs the first time
void CmdDriveRevGyroPitchDown::Initialize() 
{
  Robot::m_drivetrain->ResetEncoders();


	
	if( m_timeout > 0.0)
	{
		SetTimeout (m_timeout);
	}

}

// Called repeatedly when this Command is scheduled to run
void CmdDriveRevGyroPitchDown::Execute() 
{
	double errorAngle = Robot::m_drivetrain->GetGyroAngle() - m_heading;
	double kp = 0.04;


	Robot::m_drivetrain->Drive(m_power - errorAngle*kp  ,  m_power + errorAngle*kp ); 
	//Robot::m_drivetrain->DriveAcc(m_power - errorAngle*kp  ,  m_power + errorAngle*kp ); 
}

// Make this return true when this Command no longer needs to run execute()
bool CmdDriveRevGyroPitchDown::IsFinished() 
{
	double curr_pitch = Robot::m_drivetrain->GetGyroPitch();


	if(  (curr_pitch < PITCH_LIMIT) )
	  return true;

	if ((m_timeout>0.0) && IsTimedOut())
	{
		std::cout<<"CmdDriveFwdGyroPitchDown: Timeout"<<std::endl;
		return true;
	}


	// double l_dir = Robot::m_drivetrain->GetLeftEncoder()/Drivetrain::ENC_TICKS_PER_INCH;
	// double r_dir = Robot::m_drivetrain->GetRightEncoder()/Drivetrain::ENC_TICKS_PER_INCH;

	// if(  (l_dir < m_distance) || (r_dir < m_distance)  )
	//   return true;

	// if ((m_timeout>0.0) && IsTimedOut())
	// {
	// 	std::cout<<"CmdDriveRevGyro: Timeout"<<std::endl;
	// 	return true;
	// }

	

	return false;
}

// Called once after isFinished returns true
void CmdDriveRevGyroPitchDown::End() 
{
  if(m_stop)
	{
		Robot::m_drivetrain->Stop();
	}
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdDriveRevGyroPitchDown::Interrupted() 
{
  End();
}
