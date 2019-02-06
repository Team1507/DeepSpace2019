#pragma once

#include <frc/Commands/Subsystem.h>
#include "frc/WPILib.h"
#include "AHRS.h"

#include "ctre/Phoenix.h"


class Drivetrain : public frc::Subsystem {
 private:
  TalonSRX* leftDriveTalon;
  VictorSPX* leftDriveVictor;
  
  TalonSRX* rightDriveTalon;
  VictorSPX* rightDriveVictor;

  frc::DoubleSolenoid     *gearShift;
  frc::DoubleSolenoid     *stilts;

  AHRS *ahrs;	    //NavX

  frc::AnalogInput* analog0;
  frc::AnalogInput* analog1; 
  frc::AnalogInput* analog2; 

  unsigned char m_currLineState; //to make kris happy :)
  bool lineSensorsDeployed;


 public:

  bool IsPhotoeyeDetected(void);
    
  const static int LO_GEAR;
  const static int HI_GEAR;
  const static int DEPLOY_STILTS;
  const static int RETRACT_STILTS;
  const static int ENC_TICKS_PER_INCH;

  Drivetrain();
  void Init(void);
  void InitDefaultCommand() override;

  //*****Our Functions******
  //Encoders
	int  GetLeftEncoder(void);
	int  GetRightEncoder(void);
	void ResetEncoders(void);
  
  //Periodic
  void   DrivetrainPeriodic(void);

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

  //Line Sensor/Follower
  bool LineFollower(void);
  void LineSensorsRetract(void);
  void LineSensorsDeploy(void);

  //Tranny
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
