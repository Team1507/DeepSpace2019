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
    AHRS *ahrs;	    //NavX
    //Line Sensor Pins
    frc::AnalogInput* analog0;
    frc::AnalogInput* analog1; 
    frc::AnalogInput* analog2; 
    //variables
    
    
    unsigned char m_currLineState;
    
 public:    

    bool lineSensorsDeployed;

    const static int LO_GEAR;
    const static int HI_GEAR;
    const static double ENC_TICKS_PER_INCH;

    Drivetrain();
    void Init(void);
    void InitDefaultCommand() override;
    void DrivetrainPeriodic(void);
    
    
    //*****Our Functions******

    //Encoders
    int  GetLeftEncoder(void);
    int  GetRightEncoder(void);
    void ResetEncoders(void);
    
    //Drive
    void   DriveWithGamepad( void );
    void   Drive( double left, double right );
    void   Stop( void );
    double GetRightMotor(void);
    double GetLeftMotor(void);
    
    //Custom Arcade drive Function
    void   CustomArcadeDrive(double xSpeed, double zRotation, bool squareInputs);
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
    //bool IsLineSensorDeployed(void); BL is upset this didn't work :(
    
    //Transmition Control
    void SetLowGear(void);
    void SetHighGear(void);
    void SetGear(int gear);
    bool IsLowGear(void);
    
    //line state
    unsigned char lineStateReturn(void);
};
