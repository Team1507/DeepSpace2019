#include "subsystems/Drivetrain.h"
#include "Robot.h"
#include "Subsystems\Drivetrain.h"
#include <math.h>
#include <iostream>
//#include "Commands\CmdDriveWithGamepad.h"
#include "C:/Users/Admin/navx-mxp/cpp/include/AHRS.h"
#include "OI.h"
#include "GamepadMap.h"


//Line Follower State Machine
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

frc::DigitalInput *wallPhotoeye;


Drivetrain::Drivetrain() : Subsystem("Drivetrain") 
{
	std::cout << "In Drivetrain" << std::endl;

	leftDriveTalon = new TalonSRX(7); 
  	leftDriveVictor = new VictorSPX(8); 
 	rightDriveTalon = new TalonSRX(9);
 	rightDriveVictor = new VictorSPX(10);

  	wallPhotoeye = new frc::DigitalInput(6);
		
	
	gearShift         = new frc::DoubleSolenoid(1, 0, 1);
	stilts            = new frc::DoubleSolenoid(1, 0, 1);


	analog0 = new frc::AnalogInput(0);
  	analog1 = new frc::AnalogInput(1);
 	analog2 = new frc::AnalogInput(2);

	wallPhotoeye = new frc::DigitalInput(6);

	ahrs  	= new AHRS(SPI::Port::kMXP);
	
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


void Drivetrain::InitDefaultCommand() {
  //SetDefaultCommand(new CmdDriveWithGamepad() );
}


int	Drivetrain::GetLeftEncoder(void)
{
	return 0;//HEY DUMMY PUT THE ENCODER STUFF HERE
}
int Drivetrain::GetRightEncoder(void)
{
	return 0;//HEY DUMMY PUT THE ENCODER STUFF HERE
}


//**************************************************************
void Drivetrain::ResetEncoders(void)
{
	// leftEncoder->Reset();
	// rightEncoder->Reset();
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

	//Write Gyro to dashboard
	frc::SmartDashboard::PutNumber("GyroAngle", GetGyroAngle());
	
	frc::SmartDashboard::PutNumber("wallPhotoeye", wallPhotoeye->Get());
}



//**************************************************************
void Drivetrain::DriveWithGamepad( void )
{
	//return;
	const double deadband = 0.08;
	
	double yL = Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_Y);
	double xL = Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_X);
	double yR = Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_Y);
	double xR = -(Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_X)); //ben did this to fix L being R and R being L
	double tL = Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_L_TRIG);
	double tR = Robot::m_oi->DriverGamepad()->GetRawAxis(GAMEPADMAP_AXIS_R_TRIG);

	if (fabs(yL)<= deadband) yL = 0;
	if (fabs(xL)<= deadband) xL = 0;
	if (fabs(yR)<= deadband) yR = 0;
	if (fabs(xR)<= deadband) xR = 0;


	if(tL >= .5) //if LEFT Trigger pushed, Enable Line Follow if line is detected 
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
		//HEY REPLACE WITH THE NEW ARCADE DRIVE LINE
	}
		
	// 	Arcade Drive
	//differentialDrive->ArcadeDrive(yL,xR,  true); 
	//^We have no idea what to put there for the talons
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
				std::cout<<"ENTERING FOLLOW STATE"<<std::endl;
				driveState = STATE_LINE_FOLLOW; //follow it
			}
			return false;
			break;
		
		case STATE_LINE_FOLLOW:	
			if((m_currLineState == 0) || (m_currLineState == 111))//if the line lost/confused back to hunt
			{
				driveState = STATE_LINE_HUNT; //go back to hunt
				std::cout<<"Line lost, Leaving Follow."<<std::endl;
				return false;
			}
			else
			{
				frc::SmartDashboard::PutNumber("Current Line State", m_currLineState);
				//RUMBLE TIME BABYYYYYYYYY WOOOOOO
				//pretty rainbow lights baby woo (not as cool as rumble but whatever)
				switch(m_currLineState) //This is the switch for the actual line following
				{
					//THIS IS THE NIGHTMARE OF ALL THE CRAZY CASES. if 101...panic
					case 100:
						 leftFollowThrottle  = BASE_THROTTLE - THROTTLE_ADJUSTMENT; //was 2*throttle_adjust
						rightFollowThrottle  = BASE_THROTTLE + THROTTLE_ADJUSTMENT; //was 2*throttle_adjust
						break;
					case 110:
						leftFollowThrottle  =  BASE_THROTTLE; //- THROTTLE_ADJUSTMENT;
						rightFollowThrottle =  BASE_THROTTLE + THROTTLE_ADJUSTMENT;                  //SHOULD be a little less but we might not need to change it
						break;
					case 10:
						leftFollowThrottle = BASE_THROTTLE;
						rightFollowThrottle = BASE_THROTTLE;
						break;
					case 11:
						leftFollowThrottle  = BASE_THROTTLE + THROTTLE_ADJUSTMENT;
						rightFollowThrottle = BASE_THROTTLE; //- THROTTLE_ADJUSTMENT;               //SHOULD be a little less but we might not need to change it
						break;
					case 1:
						leftFollowThrottle  = BASE_THROTTLE + THROTTLE_ADJUSTMENT; //was 2*throttle_adjust
						rightFollowThrottle = BASE_THROTTLE - THROTTLE_ADJUSTMENT; //was 2*throttle_adjust
						break;
					case 101:
						driveState = STATE_LINE_HUNT;
						std::cout<<"OH MY GOODNESS PANIC 101 POTENTIAL SENSOR FAILURE BLAME BUILD TEAM"<<std::endl;
						return false;
						break;
					default:
						driveState = STATE_LINE_HUNT; //panic
						std::cout<<"oh my an invalid state oh no"<<std::endl;
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
//**************** TRANNY *********************

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
//********PhotoEye***********
bool Drivetrain::IsPhotoeyeDetected(void)
{
	return wallPhotoeye->Get();
}