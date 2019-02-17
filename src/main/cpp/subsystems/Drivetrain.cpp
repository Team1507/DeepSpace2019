#include "subsystems/Drivetrain.h"
#include "Robot.h"
#include <math.h>
#include <iostream>
#include "C:/Users/Admin/navx-mxp/cpp/include/AHRS.h"
#include "OI.h"
#include "GamepadMap.h"
#include "subsystems/DriverFeedback.h"
#include "commands/CmdDriveWithGamepad.h"

//Constant Defines
#define STATE_LINE_HUNT 	 0
#define STATE_LINE_FOLLOW  	 1
#define BASE_THROTTLE      	.35  //.4	
#define THROTTLE_ADJUSTMENT .08
#define THROTTLE_MULTIPLIER	1.0
#define LIMIT 1.0

//Drivetrain Constants
const int Drivetrain::LO_GEAR = 0;
const int Drivetrain::HI_GEAR = 1;
const int Drivetrain::ENC_TICKS_PER_INCH = 42;

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
	
    ahrs  	= new AHRS(SPI::Port::kMXP);
	
    analog0 = new frc::AnalogInput(0);
  	analog1 = new frc::AnalogInput(1);
 	analog2 = new frc::AnalogInput(2);	
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
}


int	Drivetrain::GetLeftEncoder(void)
{
	return leftDriveTalon->GetSensorCollection().GetQuadraturePosition();
}

int Drivetrain::GetRightEncoder(void)
{
	return rightDriveTalon->GetSensorCollection().GetQuadraturePosition();
}

void Drivetrain::ResetEncoders(void)
{
	leftDriveTalon->SetSelectedSensorPosition(0,0,0);
	rightDriveTalon->SetSelectedSensorPosition(0,0,0);
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
	//bool   bL = Robot::m_oi->DriverGamepad()->GetRawButton(GAMEPADMAP_BUTTON_LBUMP);
	bool   bL = false;
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

	//**********Line Follower Code***********
	if(bL) //if LEFT bumper pushed, Enable Line Follow if line is detected 
	{
		//If Sensors not deployed, deploy them
		if(!lineSensorsDeployed)
		{
			LineSensorsDeploy();
		}
		//If LineFolower returns False (line not detected), use Gamepad for drive
		if(!LineFollower())
		{
			//differentialDrive->ArcadeDrive(yL,xR, true);
			//HEY REPLACE WITH THE NEW ARCADE DRIVE LINE
		}
	}
	else 
	{
		//If sensors deployed, retract them
		if( lineSensorsDeployed )
			LineSensorsRetract();
		//Use Gamepad to drive
		Drive(yL, yR);
		//differentialDrive->ArcadeDrive(yL,xR, true);
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
	Drive(Limit1507(leftMotorOutput), Limit1507(rightMotorOutput));

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

unsigned char Drivetrain::lineStateReturn(void){
	return m_currLineState;
}
double Limit1507 (double x)
{
	if (x > LIMIT) x = LIMIT;
	if (x < -LIMIT) x = -LIMIT;
	return x;
}