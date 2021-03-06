#include "subsystems/Drivetrain.h"
#include "Robot.h"
#include <math.h>
#include <iostream>
//#include "C:/Users/Admin/navx-mxp/cpp/include/AHRS.h"
#include "AHRS.h"
#include "OI.h"
#include "GamepadMap.h"
#include "subsystems/DriverFeedback.h"
#include "commands/CmdDriveWithGamepad.h"

//Constant Defines
#define LIMIT 0.91507					//Arcade Drive power Limiter //was 0.8125

//Drivetrain Constants
const int Drivetrain::LO_GEAR = 0;
const int Drivetrain::HI_GEAR = 1;
const double Drivetrain::ENC_TICKS_PER_INCH = 34.43;

//Local Prototypes
double Limit1507 (double x);

Drivetrain::Drivetrain() : Subsystem("Drivetrain") 
{
    std::cout << "In Drivetrain" << std::endl;

	leftDriveTalon   = new TalonSRX(7); 
  	leftDriveVictor  = new VictorSPX(8); 
 	rightDriveTalon  = new TalonSRX(9);
 	rightDriveVictor = new VictorSPX(10);

	gearShift         = new frc::DoubleSolenoid(1, 0, 1);	
//	linesensors       = new frc::DoubleSolenoid(1, 4, 5);	

	
    ahrs  	= new AHRS(SPI::Port::kMXP);
	
    // analog0 = new frc::AnalogInput(0);
  	// analog1 = new frc::AnalogInput(1);
 	// analog2 = new frc::AnalogInput(2);	
}

void Drivetrain::Init(void)
{
	//Reset Talons
	leftDriveTalon->ConfigFactoryDefault();
	rightDriveTalon->ConfigFactoryDefault();
	leftDriveVictor->ConfigFactoryDefault();
	rightDriveVictor->ConfigFactoryDefault();
	
	//Set Neutral Mode
	leftDriveTalon->SetNeutralMode(NeutralMode::Brake);
  	rightDriveTalon->SetNeutralMode(NeutralMode::Brake);
	leftDriveVictor->SetNeutralMode(NeutralMode::Brake);
  	rightDriveVictor->SetNeutralMode(NeutralMode::Brake);

  	//Encoder Configuration
  	leftDriveTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder,0,0);
  	rightDriveTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder,0,0);
	
	//Compensation for inverted wires
	leftDriveTalon->SetInverted(false);
	rightDriveTalon->SetInverted(true);
	leftDriveVictor->SetInverted(false);
	rightDriveVictor->SetInverted(true);
	
	//Start Following
	leftDriveVictor->Follow(*leftDriveTalon);
  	rightDriveVictor->Follow(*rightDriveTalon);  
}


void Drivetrain::InitDefaultCommand()
{
	SetDefaultCommand(new CmdDriveWithGamepad());
}


void Drivetrain::DrivetrainPeriodic(void)
{
}


int	Drivetrain::GetLeftEncoder(void)
{
	return leftDriveTalon->GetSensorCollection().GetQuadraturePosition()*(-1.0);
}

int Drivetrain::GetRightEncoder(void)
{
	return rightDriveTalon->GetSensorCollection().GetQuadraturePosition();
}

void Drivetrain::ResetEncoders(void)
{
	leftDriveTalon->SetSelectedSensorPosition(0,0,0);
	rightDriveTalon->SetSelectedSensorPosition(0,0,0);
	std::cout << "Reset Encoders" << std::endl;
}

//~~~~~~~~~~~~DRIVE WITH GAMEPAD~~~~~~~~~~~~~~
void Drivetrain::DriveWithGamepad( void )
{
	const double deadzone = 0.08;
	
	double yL = Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_Y);
	double xL = Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_X);
	double yR = Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_Y);
	double xR = -(Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_X)); //ben did this to fix L being R and R being L
	double tL = Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_TRIG);
	double tR = Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_TRIG);
	bool   bL = Robot::m_oi->DriverGamepad()->GetRawButton(GAMEPADMAP_BUTTON_LBUMP);
	//bool   bL = false;
	
	if (fabs(yL)<= deadzone) yL = 0;
	if (fabs(xL)<= deadzone) xL = 0;
	if (fabs(yR)<= deadzone) yR = 0;
	if (fabs(xR)<= deadzone) xR = 0;

	//**************Gear Shift****************
	if (Robot::m_oi->DriverGamepad()->GetRawButtonPressed(GAMEPADMAP_BUTTON_RBUMP))
	{
		SetHighGear();
		std::cout<<"HighGear"<<std::endl;
	}
	if (tR > 0.5)
	{
		SetLowGear();
		std::cout<<"LowGear"<<std::endl;
	}


	//** Call Drive Code
	CustomArcadeDrive(yL,xR, true);		//FPS Drive
	//Drive(yL, yR);					//Tank Drive


}

//**************************************************************
void Drivetrain::Drive( double left, double right )
{
	leftDriveTalon->Set(ControlMode::PercentOutput, left*(-1.0));
	rightDriveTalon->Set(ControlMode::PercentOutput, right*(-1.0));
}
void Drivetrain::Stop( void )
{
	leftDriveTalon->Set(ControlMode::PercentOutput, 0);
	rightDriveTalon->Set(ControlMode::PercentOutput, 0);
  	std::cout << "DRIVE MOTORS STOPPED" << std::endl;
}
double Drivetrain::GetRightMotor(void)
{
	return rightDriveTalon->GetMotorOutputPercent();
}
double Drivetrain::GetLeftMotor(void)
{
	return leftDriveTalon->GetMotorOutputPercent();
}

//********************Custom Arcade Drive************************
void Drivetrain::CustomArcadeDrive(double xSpeed, double zRotation, bool squareInputs)
{
	xSpeed = Limit1507(xSpeed);

	zRotation = Limit1507(zRotation);

	//Square the inputs (while preserving the sign) to increase fine control while permitting full power.
	if (squareInputs) {
		xSpeed = std::copysign(xSpeed * xSpeed, xSpeed);
		zRotation = std::copysign(zRotation * zRotation, zRotation);
	}

	double leftMotorOutput;
	double rightMotorOutput;

	double maxInput = std::copysign(std::max(fabs(xSpeed), fabs(zRotation)), xSpeed);

	if (xSpeed >= 0.0)
	{
		if (zRotation >= 0.0) //First quandrant, else second quadrant
		{
			leftMotorOutput = maxInput;
			rightMotorOutput = xSpeed - zRotation;
		}
		else
		{
			leftMotorOutput = xSpeed + zRotation;
			rightMotorOutput = maxInput;
		}
	}
	else //Third quadrant, else fourth quadrant
	{
		if (zRotation >= 0.0) 
		{
			leftMotorOutput = xSpeed + zRotation;
			rightMotorOutput = maxInput;
		}
		else
		{
			leftMotorOutput = maxInput;
			rightMotorOutput = xSpeed - zRotation;
		}
	}
	Drive(Limit1507(leftMotorOutput)*(-1.0), Limit1507(rightMotorOutput)*(-1.0));

}
//**************** AHRS (NavX) *********************
bool Drivetrain::IsGyroConnected(void)
{
	return ahrs->IsConnected();
}
double Drivetrain::GetGyroYaw(void)
{
    //Returns Relative Yaw:  -180 to +180
	return (double) ahrs->GetYaw();
}
double Drivetrain::GetGyroAngle(void)
{
    //returns total accumulated angle -inf to +inf  (continuous through 360deg)
	return (double) ahrs->GetAngle();
}
double Drivetrain::GetGyroRate(void)
{
	return ahrs->GetRate();
}
double Drivetrain::GetGyroPitch(void)
{
	return (double) ahrs->GetPitch();
}
void Drivetrain::ZeroGyro(void)
{
    std::cout<<"ZeroGyro"<<std::endl;
    ahrs->ZeroYaw();
	//**OR
	//ahrs->Reset();//????
}


//**************** Line Follower *********************

bool Drivetrain::LineFollower(void)
{
	//Perminantly disable Line Following
	return false;
	// static unsigned char driveState = STATE_LINE_HUNT;
	// double leftFollowThrottle;
	// double rightFollowThrottle;

	// switch(driveState)
	// {
		
	// 	case STATE_LINE_HUNT:
	// 		if((m_currLineState > 0) && (m_currLineState < 111))//if the line is found
	// 		{
	// 			std::cout<<"Line Found."<<std::endl;
	// 			Robot::m_driverfeedback->RumbleOn();
	// 			driveState = STATE_LINE_FOLLOW; //follow it
	// 		}
	// 		return false;
	// 		break;
		
	// 	case STATE_LINE_FOLLOW:	
	// 		if((m_currLineState == 0) || (m_currLineState == 111))//if the line lost/confused back to hunt
	// 		{
	// 			driveState = STATE_LINE_HUNT; //go back to hunt
	// 			std::cout<<"Line lost, Leaving Follow."<<std::endl;
	// 			Robot::m_driverfeedback->UpdateLeftLEDs(BLUE_rgb);
	// 			Robot::m_driverfeedback->UpdateRightLEDs(BLUE_rgb);
	// 			Robot::m_driverfeedback->RumbleOff();
	// 			return false;
	// 		}
	// 		else
	// 		{
	// 			frc::SmartDashboard::PutNumber("Current Line State", m_currLineState);
	// 			switch(m_currLineState) //This is the switch for the actual line following
	// 			{
	// 				//THIS IS THE NIGHTMARE OF ALL THE CRAZY CASES. if 101...panic
	// 				case 100:
	// 					leftFollowThrottle  = BASE_THROTTLE - THROTTLE_ADJUSTMENT; //was 2*throttle_adjust
	// 					rightFollowThrottle  = BASE_THROTTLE + THROTTLE_ADJUSTMENT; //was 2*throttle_adjust
	// 					Robot::m_driverfeedback->UpdateLeftLEDs(RED_rgb);
	// 					Robot::m_driverfeedback->RightLEDsOff();
	// 					break;
	// 				case 110:
	// 					leftFollowThrottle  =  BASE_THROTTLE; //- THROTTLE_ADJUSTMENT;
	// 					rightFollowThrottle =  BASE_THROTTLE + THROTTLE_ADJUSTMENT;            //SHOULD be a little less but we might not need to change it
	// 					Robot::m_driverfeedback->UpdateLeftLEDs(YELLOW_rgb);
	// 					Robot::m_driverfeedback->RightLEDsOff();
	// 					break;
	// 				case 10:
	// 					leftFollowThrottle = BASE_THROTTLE;
	// 					rightFollowThrottle = BASE_THROTTLE;
	// 					Robot::m_driverfeedback->UpdateLeftLEDs(GREEN_rgb);
	// 					Robot::m_driverfeedback->UpdateRightLEDs(GREEN_rgb);
	// 					break;
	// 				case 11:
	// 					leftFollowThrottle  = BASE_THROTTLE + THROTTLE_ADJUSTMENT;
	// 					rightFollowThrottle = BASE_THROTTLE; //- THROTTLE_ADJUSTMENT;         //SHOULD be a little less but we might not need to change it
	// 					Robot::m_driverfeedback->LeftLEDsOff();
	// 					Robot::m_driverfeedback->UpdateRightLEDs(YELLOW_rgb);
	// 					break;
	// 				case 1:
	// 					leftFollowThrottle  = BASE_THROTTLE + THROTTLE_ADJUSTMENT; //was 2*throttle_adjust
	// 					rightFollowThrottle = BASE_THROTTLE - THROTTLE_ADJUSTMENT; //was 2*throttle_adjust
	// 					Robot::m_driverfeedback->LeftLEDsOff();
	// 					Robot::m_driverfeedback->UpdateRightLEDs(RED_rgb);
	// 					break;
	// 				case 101:
	// 					driveState = STATE_LINE_HUNT;
	// 					std::cout<<"OH MY GOODNESS PANIC 101 POTENTIAL SENSOR FAILURE BLAME BUILD TEAM"<<std::endl;
	// 					Robot::m_driverfeedback->LeftLEDsOff();
	// 					Robot::m_driverfeedback->RightLEDsOff();
	// 					return false;
	// 					break;
	// 				default:
	// 					driveState = STATE_LINE_HUNT; //panic
	// 					std::cout<<"oh my an invalid state oh no"<<std::endl;
	// 					Robot::m_driverfeedback->LeftLEDsOff();
	// 					Robot::m_driverfeedback->RightLEDsOff();
	// 					return false;
	// 			}
	// 			frc::SmartDashboard::PutNumber("LEFT THROTTLE", leftFollowThrottle);
	// 			frc::SmartDashboard::PutNumber("RIGHT THROTTLE", rightFollowThrottle);
	// 			Drive(leftFollowThrottle*(THROTTLE_MULTIPLIER), rightFollowThrottle*(THROTTLE_MULTIPLIER));	//was -1,-1
	// 		}
	// 		break;

	// 	default:
	// 		driveState = STATE_LINE_HUNT; //failsafe
	// 		return false;
	// 		break;

	// }
	// frc::SmartDashboard::PutNumber("LINE FOLLOW STATE", driveState);
	// return true;

}

//Line Sensors Deploy/Retract
void Drivetrain::LineSensorsRetract(void)
{
	// linesensors->Set(DoubleSolenoid::kReverse);
	// lineSensorsDeployed = false;
	// std::cout << "Line Sensors Retract" << std::endl;
}
void Drivetrain::LineSensorsDeploy(void)
{
	// linesensors->Set(DoubleSolenoid::kForward);
	// lineSensorsDeployed = true;
	// std::cout << "Line Sensors Deploy" << std::endl;
}




//****************** TRANNY **************************

void Drivetrain::SetLowGear( void )
{
	gearShift->Set(DoubleSolenoid::kForward);
	std::cout << "LowGear" << std::endl;
}
void Drivetrain::SetHighGear( void )
{
	gearShift->Set(DoubleSolenoid::kReverse);
	std::cout << "HighGear" << std::endl;
}

void Drivetrain::SetGear(int gear)
{
	if( gear == LO_GEAR ) SetLowGear();
	if( gear == HI_GEAR ) SetHighGear();
}

bool Drivetrain::IsLowGear(void)
{
	DoubleSolenoid::Value CurrGear = (gearShift->Get());

	if( CurrGear == DoubleSolenoid::kForward)
		return true;
	else
		return false;
  	return false;
}

unsigned char Drivetrain::lineStateReturn(void){
	return m_currLineState;
}
double Limit1507 (double x)
{
	if (x > LIMIT) x = LIMIT;
	if (x < -LIMIT) x = -LIMIT;
	return x;
}