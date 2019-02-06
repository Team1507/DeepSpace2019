

#include "Commands\CmdDriveFwdGyroPitchDown.h"
#include "Robot.h"

const double PITCH_LIMIT = -15.0;

CmdDriveFwdGyroPitchDown::CmdDriveFwdGyroPitchDown(double power, double heading, double distance, bool stop, double timeout)
{
  m_power    = power;
	m_heading  = heading;
  m_distance = distance;
  m_stop     = stop;
  m_timeout  = timeout;

  // Use Requires() here to declare subsystem dependencies
  Requires( Robot::m_drivetrain);
}

// Called just before this Command runs the first time
void CmdDriveFwdGyroPitchDown::Initialize() 
{
  Robot::m_drivetrain->ResetEncoders();

	if( m_timeout > 0.0)
	{
		SetTimeout (m_timeout);
	}

}

// Called repeatedly when this Command is scheduled to run
void CmdDriveFwdGyroPitchDown::Execute() 
{
	//Don't use GetGyroYaw; if we try to drive on -181 heading, yaw freaks out and goes positive
	//double errorAngle = Robot::m_drivetrain->GetGyroYaw() - m_heading;
	double errorAngle = Robot::m_drivetrain->GetGyroAngle() - m_heading;
	double kp = 0.04; //was 0.05

	Robot::m_drivetrain->Drive(m_power - errorAngle*kp  ,  m_power + errorAngle*kp ); 
	//Robot::m_drivetrain->DriveAcc(m_power - errorAngle*kp  ,  m_power + errorAngle*kp ); 
}

// Make this return true when this Command no longer needs to run execute()
bool CmdDriveFwdGyroPitchDown::IsFinished() 
{

	double curr_pitch = Robot::m_drivetrain->GetGyroPitch();


	if(  (curr_pitch < PITCH_LIMIT) )
	  return true;

	if ((m_timeout>0.0) && IsTimedOut())
	{
		std::cout<<"CmdDriveFwdGyroPitchDown: Timeout"<<std::endl;
		return true;
	}

	return false;
}

// Called once after isFinished returns true
void CmdDriveFwdGyroPitchDown::End() 
{
  if(m_stop)
	{
		std::cout<<"GyroAngle ";
		std::cout<<Robot::m_drivetrain->GetGyroAngle()<<std::endl;
		std::cout<<"EncoderValueL ";
		std::cout<<Robot::m_drivetrain->GetLeftEncoder()<<std::endl;
		std::cout<<"EncoderValueR ";
		std::cout<<Robot::m_drivetrain->GetRightEncoder()<<std::endl;
		Robot::m_drivetrain->Stop();
	}
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdDriveFwdGyroPitchDown::Interrupted() 
{
  End();
}
