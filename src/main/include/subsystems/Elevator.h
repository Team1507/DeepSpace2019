


#ifndef ELEVATOR_H
#define ELEVATOR_H
#include "frc/commands/Subsystem.h"
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"

class Elevator: public frc::Subsystem {
private:
	
	TalonSRX* elevatorTalonSRX;
	VictorSPX* elevatorVictorSPX;

	int reqValue;

public:
	Elevator();
	void InitDefaultCommand() override;
	void ElevatorPeriodic(void);

	void TalonSRXinit(void);
	void ElevatorMotorDrive(double power);
	void Stop(void);
	int  GetElevatorEncoder(void);
	bool GetElevatorLimitSwitchTop(void);
	bool GetElevatorLimitSwitchBot(void);
	void ClearElevatorEncoder(void);
	double GetElevatorMotorDrive(void);
	void SetReqElevatorValue(int value);
	void ClearReqElevatorValue(void);

	const static int BOT_VALUE;
	const static int CARGO_VALUE; 	
	const static int ROCKET_MID_VALUE;
	const static int ROCKET_HIGH_VALUE;  
	const static double UP_DRIVE;          
	const static double DOWN_DRIVE;        
	const static double HOLD_DRIVE;
	const static double HOME_DRIVE;                      
	const static int ENCODER_TOLERANCE;     
};

#endif
