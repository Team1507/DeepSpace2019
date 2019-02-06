#pragma once

#include <frc/TimedRobot.h>
#include <frc/commands/Command.h>
#include <frc/smartdashboard/SendableChooser.h>

#include "OI.h"
#include "subsystems/Drivetrain.h"
#include "subsystems/Collector.h"
#include "subsystems/Carriage.h"
#include "subsystems/Elevator.h"

#include "AHRS.h"

#include <iostream>	
#include <iomanip>  //std::setprecision

class Robot : public frc::TimedRobot {
 public:
    static OI *m_oi;
    AHRS *ahrs;
  
    //Subsystems
    static Drivetrain *m_drivetrain;
    static Collector *m_collector;
    static Carriage *m_carriage;
    static Elevator *m_elevator;

    void RobotInit() override;
    void RobotPeriodic() override;
    void DisabledInit() override;
    void DisabledPeriodic() override;
    void AutonomousInit() override;
    void AutonomousPeriodic() override;
    void TeleopInit() override;
    void TeleopPeriodic() override;
    void TestPeriodic() override;

 private:
    frc::Command* m_autonomousCommand = nullptr;
    frc::SendableChooser<frc::Command*> m_chooser;
};
