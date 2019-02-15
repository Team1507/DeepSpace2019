#include "subsystems/Drivetrain.h"
#include "Robot.h"
#include <math.h>
#include <iostream>
#include "C:/Users/Admin/navx-mxp/cpp/include/AHRS.h"
#include "OI.h"
#include "GamepadMap.h"
#include "commands/GrpDriveOffHAB.h"
#include "commands/GrpDriveOnHAB.h"
#include "subsystems/DriverFeedback.h"


//Line Follower State Machine defines
#define STATE_LINE_HUNT 	 0
#define STATE_LINE_FOLLOW  	 1
#define BASE_THROTTLE      	.35  //.4	
#define THROTTLE_ADJUSTMENT .08
#define THROTTLE_MULTIPLIER	1.0

//Drivetrain Constants
const int Drivetrain::LO_GEAR = 0;
const int Drivetrain::HI_GEAR = 1;
const int Drivetrain::RETRACT_STILTS = 0;
const int Drivetrain::DEPLOY_STILTS  = 1;
const int Drivetrain::ENC_TICKS_PER_INCH = 42;


Drivetrain::Drivetrain() : Subsystem("Drivetrain") 
{
    std::cout << "In Drivetrain" << std::endl;

	leftDriveTalon   = new TalonSRX(7); 
  	leftDriveVictor  = new VictorSPX(8); 
 	rightDriveTalon  = new TalonSRX(9);
 	rightDriveVictor = new VictorSPX(10);

	//DifferentialDrive

	gearShift         = new frc::DoubleSolenoid(1, 0, 1);	
	stilts            = new frc::DoubleSolenoid(1, 0, 1);
	wallPhotoeye      = new frc::DigitalInput(6);
    
    ahrs  	= new AHRS(SPI::Port::kMXP);
	
    analog0 = new frc::AnalogInput(0);
  	analog1 = new frc::AnalogInput(1);
 	analog2 = new frc::AnalogInput(2);	
}

void Drivetrain::Init(void)
{
	leftDriveTalon->SetNeutralMode(NeutralMode::Brake);
  	rightDriveTalon->SetNeutralMode(NeutralMode::Brake);

  	//encoder code
  	leftDriveTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder,0,0);
  	rightDriveTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder,0,0);
	
	leftDriveVictor->Follow(*leftDriveTalon);
  	rightDriveVictor->Follow(*rightDriveTalon);  
}

void Drivetrain::InitDefaultCommand() 
{
}

void Drivetrain::DrivetrainPeriodic(void)
{
	//****************Line Follower Sensor ********************
	double voltage0 = analog0->GetVoltage() ;
  	double voltage1 = analog1->GetVoltage() ;
  	double voltage2 = analog2->GetVoltage() ;
  
	frc::SmartDashboard::PutNumber("v0", voltage0);
	frc::SmartDashboard::PutNumber("v1", voltage1);
	frc::SmartDashboard::PutNumber("v2", voltage2);

	bool leftEye;
	bool centerEye;
	bool rightEye;

	const double threshold = 3.2;
	
	if(voltage0 <= threshold) leftEye   = true; else (leftEye   = false);
	if(voltage1 <= threshold) centerEye = true; else (centerEye = false);
	if(voltage2 <= threshold) rightEye  = true; else (rightEye  = false);

	frc::SmartDashboard::PutBoolean("L", leftEye);
	frc::SmartDashboard::PutBoolean("C", centerEye);
	frc::SmartDashboard::PutBoolean("R", rightEye);
	frc::SmartDashboard::PutBoolean("Deployed", lineSensorsDeployed);
	
	int currState = 0;
	if(leftEye)  currState = currState + 100;
	if(centerEye)currState = currState + 10;
	if(rightEye) currState = currState + 1;
	m_currLineState = currState;

	//Write Gyro and photoeye state to dashboard
	frc::SmartDashboard::PutNumber("GyroAngle", GetGyroAngle());
	frc::SmartDashboard::PutNumber("wallPhotoeye", wallPhotoeye->Get());

	//*************

}

int	Drivetrain::GetLeftEncoder(void)
{
	return 0;//HEY DUMMY PUT THE ENCODER STUFF HERE
}

int Drivetrain::GetRightEncoder(void)
{
	return 0;//HEY DUMMY PUT THE ENCODER STUFF HERE
}

void Drivetrain::ResetEncoders(void)
{
	// leftEncoder->Reset();
	// rightEncoder->Reset();
}

bool Drivetrain::IsPhotoeyeDetected(void)
{
	return wallPhotoeye->Get();
}
void Drivetrain::DriveWithGamepad( void )
{
	//return;
	const double deadzone = 0.08;
	
	double yL = Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_Y);
	double xL = Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_X);
	double yR = Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_Y);
	double xR = -(Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_X)); //ben did this to fix L being R and R being L
	double tL = Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_TRIG);
	double tR = Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_TRIG);
	bool   bL = Robot::m_oi->DriverGamepad()->GetRawButton(GAMEPADMAP_BUTTON_LBUMP);
	if (fabs(yL)<= deadzone) yL = 0;
	if (fabs(xL)<= deadzone) xL = 0;
	if (fabs(yR)<= deadzone) yR = 0;
	if (fabs(xR)<= deadzone) xR = 0;

	// drive str8 testing

		//static bool driveStr8_flag = false;
		//static double currHeading;

	//calculus (-2 to invert drive)
	//double driveStr8_power = (left + right)/-2.0;
//
//	//if start held, drive str8
//	//if(Robot::m_oi->OperatorGamepad()->GetRawButton(8))
//	{
//		if(driveStr8_flag)
//		{
//			//if drivestr8_flag = true
//			double errorAngle = Robot::drivetrain->GetGyroAngle() - currHeading;
//			double kp = 0.03;

//			Robot::drivetrain->Drive(driveStr8_power - errorAngle*kp , driveStr8_power + errorAngle*kp);
//		}
//		else
//		{
//			//if drivestr8_flag = false
//			driveStr8_flag = true;
//			currHeading = Robot::drivetrain->GetGyroAngle();
//			std::cout<<"DriveStr8"<<std::endl;
//		}
//
//	}
//	else{
//		//clear flag when not drive str8
//		driveStr8_flag = false;
//		//Finally, write to talons
//		//differentialDrive->ArcadeDrive( (0.9)*left,  (0.9)*right,  true);  //put a limiter for n00bs
//		//Drive(left, right);
//		if((!Robot::m_oi->OperatorGamepad()->GetRawButton(8))
//		{
//			differentialDrive->ArcadeDrive( (0.6)*left,  (0.6)*right,  true);  //changes left/right drive to 1/2 power for manuvers  //was .5,.5
//		}
//		else
//		{
//			differentialDrive->ArcadeDrive( (0.9)*left,  (0.9)*right,  true);  //put a limiter for n00bs
//		}
//	}
//


	
	//*************Extend/retract stilts*************
	double povAngle = Robot::m_oi->DriverGamepad()->GetPOV(0);
	SmartDashboard::PutNumber("DriverPOVAngle",povAngle);
	if( Robot::m_oi->DriverGamepad()->GetRawAxis(2) >= .5 && povAngle == 0.0)SetStilts(RETRACT_STILTS);
	if( Robot::m_oi->DriverGamepad()->GetRawAxis(2) >= .5 && povAngle == 270.0)SetStilts(DEPLOY_STILTS);

	//**********Auto Climb and Descend**********
	if( Robot::m_oi->DriverGamepad()->GetRawAxis(2) >= .5 && Robot::m_oi->DriverGamepad()->GetRawButton(4))GrpDriveOnHAB(); //HEY check if this is right
	if( Robot::m_oi->DriverGamepad()->GetRawAxis(2) >= .5 && Robot::m_oi->DriverGamepad()->GetRawButton(1))GrpDriveOffHAB();

	//*********Line Follower Code*************
	if(bL) //if LEFT bumper pushed, Enable Line Follow if line is detected 
	{
		//If Sensors not deployed, deploy them
		if( !lineSensorsDeployed )
			LineSensorsDeploy();

		//If LineFolower returns False (line not detected), use Gamepad for drive
		if ( !LineFollower() ){}
			//differentialDrive->ArcadeDrive(yL,xR, true);
			//HEY REPLACE WITH THE NEW ARCADE DRIVE LINE

	}
	else 
	{
		//If sensors deployed, retract them
		if( lineSensorsDeployed )
			LineSensorsRetract();
		//Use Gamepad to drive
		//differentialDrive->ArcadeDrive(yL,xR, true);
		//differentialDrive->ArcadeDrive(,xR, true);
		//HEY REPLACE WITH THE NEW ARCADE DRIVE LINE
	}
		
	// 	Arcade Drive
	//differentialDrive->ArcadeDrive(yL,xR,  true); 
	//^HEY REPLACE WITH THE NEW ARCADE DRIVE LINE
}

//**************************************************************
void Drivetrain::Drive( double left, double right )
{
	leftDriveTalon->Set(ControlMode::PercentOutput, left);
	rightDriveTalon->Set(ControlMode::PercentOutput, right);
}
void Drivetrain::Stop( void )
{
	leftDriveTalon->Set(ControlMode::PercentOutput, 0);
	rightDriveTalon->Set(ControlMode::PercentOutput, 0);
  	std::cout << "DRIVE MOTORS STOPPED" << std::endl;
}
//********************Custom Arcade Drive************************
void CustomArcadeDrive(double leftJoyY, double rightJoyX)
{
	// double leftThrottle, rightThrottle;
	// leftThrottle  = leftJoyY + rightJoyX;
	// rightThrottle =	leftJoyY + (-1)rightJoyX;
	// if(leftThrottle > 1)   leftThrottle  =  1;
	// if(leftThrottle < -1)  leftThrottle  = -1;
	// if(rightThrottle > 1)  rightThrottle =  1;
	// if(rightThrottle < -1) rightThrottle = -1;
	// Drive(leftThrottle, rightThrottle);
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
	static unsigned char driveState = STATE_LINE_HUNT;
	double leftFollowThrottle;
	double rightFollowThrottle;

	switch(driveState)
	{
		
		case STATE_LINE_HUNT:
			if((m_currLineState > 0) && (m_currLineState < 111))//if the line is found
			{
				std::cout<<"Line Found."<<std::endl;
				Robot::m_driverfeedback->RumbleOn();
				driveState = STATE_LINE_FOLLOW; //follow it
			}
			return false;
			break;
		
		case STATE_LINE_FOLLOW:	
			if((m_currLineState == 0) || (m_currLineState == 111))//if the line lost/confused back to hunt
			{
				driveState = STATE_LINE_HUNT; //go back to hunt
				std::cout<<"Line lost, Leaving Follow."<<std::endl;
				Robot::m_driverfeedback->UpdateLeftLEDs(BLUE_rgb);
				Robot::m_driverfeedback->UpdateRightLEDs(BLUE_rgb);
				Robot::m_driverfeedback->RumbleOn();
				return false;
			}
			else
			{
				frc::SmartDashboard::PutNumber("Current Line State", m_currLineState);
				//RUMBLE TIME BABYYYYYYYYY WOOOOOO
				//HEY light command stuff (not as cool as rumble but whatever)
				switch(m_currLineState) //This is the switch for the actual line following
				{
					//THIS IS THE NIGHTMARE OF ALL THE CRAZY CASES. if 101...panic
					case 100:
						leftFollowThrottle  = BASE_THROTTLE - THROTTLE_ADJUSTMENT; //was 2*throttle_adjust
						rightFollowThrottle  = BASE_THROTTLE + THROTTLE_ADJUSTMENT; //was 2*throttle_adjust
						Robot::m_driverfeedback->UpdateLeftLEDs(RED_rgb);
						Robot::m_driverfeedback->RightLEDsOff();
						break;
					case 110:
						leftFollowThrottle  =  BASE_THROTTLE; //- THROTTLE_ADJUSTMENT;
						rightFollowThrottle =  BASE_THROTTLE + THROTTLE_ADJUSTMENT;                  //SHOULD be a little less but we might not need to change it
						Robot::m_driverfeedback->UpdateLeftLEDs(YELLOW_rgb);
						Robot::m_driverfeedback->RightLEDsOff();
						break;
					case 10:
						leftFollowThrottle = BASE_THROTTLE;
						rightFollowThrottle = BASE_THROTTLE;
						Robot::m_driverfeedback->UpdateLeftLEDs(GREEN_rgb);
						Robot::m_driverfeedback->UpdateRightLEDs(GREEN_rgb);
						break;
					case 11:
						leftFollowThrottle  = BASE_THROTTLE + THROTTLE_ADJUSTMENT;
						rightFollowThrottle = BASE_THROTTLE; //- THROTTLE_ADJUSTMENT;               //SHOULD be a little less but we might not need to change it
						Robot::m_driverfeedback->LeftLEDsOff();
						Robot::m_driverfeedback->UpdateRightLEDs(YELLOW_rgb);
						break;
					case 1:
						leftFollowThrottle  = BASE_THROTTLE + THROTTLE_ADJUSTMENT; //was 2*throttle_adjust
						rightFollowThrottle = BASE_THROTTLE - THROTTLE_ADJUSTMENT; //was 2*throttle_adjust
						Robot::m_driverfeedback->LeftLEDsOff();
						Robot::m_driverfeedback->UpdateRightLEDs(RED_rgb);
						break;
					case 101:
						driveState = STATE_LINE_HUNT;
						std::cout<<"OH MY GOODNESS PANIC 101 POTENTIAL SENSOR FAILURE BLAME BUILD TEAM"<<std::endl;
						Robot::m_driverfeedback->LeftLEDsOff();
						Robot::m_driverfeedback->RightLEDsOff();
						return false;
						break;
					default:
						driveState = STATE_LINE_HUNT; //panic
						std::cout<<"oh my an invalid state oh no"<<std::endl;
						Robot::m_driverfeedback->LeftLEDsOff();
						Robot::m_driverfeedback->RightLEDsOff();
						return false;
				}
				frc::SmartDashboard::PutNumber("LEFT THROTTLE", leftFollowThrottle);
				frc::SmartDashboard::PutNumber("RIGHT THROTTLE", rightFollowThrottle);
				Drive(leftFollowThrottle*(THROTTLE_MULTIPLIER), rightFollowThrottle*(THROTTLE_MULTIPLIER));	//was -1,-1
			}
			break;

		default:
			driveState = STATE_LINE_HUNT; //failsafe
			return false;
			break;

	}
	frc::SmartDashboard::PutNumber("LINE FOLLOW STATE", driveState);
	return true;

}

void Drivetrain::LineSensorsRetract(void)
{
	//RETRACT SENSORS HERE!!
	lineSensorsDeployed = false;
	std::cout << "Line Sensors Retract" << std::endl;
}
void Drivetrain::LineSensorsDeploy(void)
{
	//DEPLOY SENSORS HERE
	lineSensorsDeployed = true;
	std::cout << "Line Sensors Deploy" << std::endl;
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

//**************** STILTS *********************
void Drivetrain::DeployStilts( void )
{
	stilts->Set(DoubleSolenoid::kForward);
	std::cout << "StiltsDeployed" << std::endl;
}
void Drivetrain::RetractStilts( void )
{
	stilts->Set(DoubleSolenoid::kReverse);
	std::cout << "StiltsRetracted" << std::endl;
}

void Drivetrain::SetStilts(int stilts)
{
	if( stilts == DEPLOY_STILTS  ) DeployStilts();
	if( stilts == RETRACT_STILTS ) RetractStilts();
}

bool Drivetrain::AreStiltsDeployed(void)
{
	DoubleSolenoid::Value StiltsPosition = (stilts->Get());

	if( StiltsPosition == DoubleSolenoid::kForward)
		return true;
	else
		return false;
  return false;
}
unsigned char Drivetrain::lineStateReturn(void){
	return m_currLineState;
}