


#ifndef ELEVATOR_H
#define ELEVATOR_H
#include "frc/commands/Subsystem.h"
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"

class Elevator: public frc::Subsystem {
private:
	
	TalonSRX* elevatorTalonSRX;

public:
	Elevator();
	void InitDefaultCommand() override;
	void Periodic() override;

	void TalonSRXinit(void);
	void ElevatorMotorDrive(double power);
	void Stop(void);
	int  GetElevatorEncoder(void);
	bool GetElevatorLimitSwitchTop(void);
	bool GetElevatorLimitSwitchBot(void);
	void ClearElevatorEncoder(void);
	double GetElevatorMotorDrive(void);
	void SetReqElevatorValue(int value);

	const static int BOT_VALUE;
	const static int TOP_VALUE;     
	const static int CARGO_VALUE; 	
	const static int ROCKET_VALUE;
	const static int CRUISE_VELOCITY;
	const static int ACCELERATION; 	
	
};

#endif
