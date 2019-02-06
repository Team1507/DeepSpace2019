#pragma once

#include <frc/Commands/Subsystem.h>
#include "frc/WPILib.h"
#include "AHRS.h"

#include "ctre/Phoenix.h"


class Drivetrain : public frc::Subsystem {
 private:
  //Object instantiation
  TalonSRX* leftDriveTalon;
  VictorSPX* leftDriveVictor;
  TalonSRX* rightDriveTalon;
  VictorSPX* rightDriveVictor;
  frc::DoubleSolenoid     *gearShift;
  frc::DoubleSolenoid     *stilts;
  frc::DigitalInput *wallPhotoeye; //Was in the .cpp, Ben L moved it because it looked funny there
  AHRS *ahrs;	    //NavX
  //Line Sensor Pins
  frc::AnalogInput* analog0;
  frc::AnalogInput* analog1; 
  frc::AnalogInput* analog2; 
  //variables
  unsigned char m_currLineState;
  bool lineSensorsDeployed;


 public:    
  const static int LO_GEAR;
  const static int HI_GEAR;
  const static int DEPLOY_STILTS;
  const static int RETRACT_STILTS;
  const static int ENC_TICKS_PER_INCH;

  Drivetrain();
  void Init(void);
  void InitDefaultCommand() override;
  void   DrivetrainPeriodic(void);
  
  
  //*****Our Functions******

  //Encoders
	int  GetLeftEncoder(void);
	int  GetRightEncoder(void);
	void ResetEncoders(void);
  //Photoeye check
  bool IsPhotoeyeDetected(void);
  //Drive
  void   DriveWithGamepad( void );
  void   Drive( double left, double right );
  void   Stop( void );
  //NavX
	bool   IsGyroConnected(void);
	double GetGyroYaw(void);            //yaw: Relative -180 to +180
	double GetGyroAngle(void);          //angle: absolute -inf to +inf
	double GetGyroRate(void);
  double GetGyroPitch(void);
	void   ZeroGyro(void);
  //Line Sensors and Follower functions
  bool LineFollower(void);
  void LineSensorsRetract(void);
  void LineSensorsDeploy(void);
  //Transmition Control
  void SetLowGear(void);
  void SetHighGear(void);
  void SetGear(int gear);
  bool IsLowGear(void);
  //Stilts
  void DeployStilts(void);
  void RetractStilts(void);
  void SetStilts(int stilts);
  bool AreStiltsDeployed(void);
};
