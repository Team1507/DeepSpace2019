#include "OI.h"
#include "commands/GrpTest2.h"
#include "commands/CmdElevatorGo2Top.h"
#include "commands/CmdElevatorGo2Bot.h"
#include "commands/CmdElevatorGo2RocketMid.h"
#include "commands/CmdElevatorGo2CargoMid.h"
#include <frc/WPILib.h>
#include "commands/CmdDriveClearAll.h"
#include "commands/CmdDriveStr8.h"
#include "commands/GrpStartLeftPlaceHatch.h"
#include "commands/GrpStartRightPlaceHatch.h"

OI::OI() {
    // Process operator interface input here.

    //Init Gamepads
    driver_gamepad = new frc::Joystick(0);
    operator_gamepad = new frc::Joystick(1);

    //frc::SmartDashboard::PutData("GrpTest2 Command", new GrpTest2() );
   
    frc::SmartDashboard::PutData("GrpStartLeftPlaceHatch", new GrpStartLeftPlaceHatch() );
    frc::SmartDashboard::PutData("GrpStartRightPlaceHatch", new GrpStartRightPlaceHatch() );

	  frc::SmartDashboard::PutData("ZeroEncoder", new CmdDriveClearAll() );
    
    frc::SmartDashboard::PutData("CmdElevatorGo2Top", new CmdElevatorGo2Top() );
    frc::SmartDashboard::PutData("CmdElevatorGo2Bot", new CmdElevatorGo2Bot() );
    frc::SmartDashboard::PutData("CmdElevatorGo2RocketMid", new CmdElevatorGo2RocketMid() );
    frc::SmartDashboard::PutData("CmdElevatorGo2CargoMid", new CmdElevatorGo2CargoMid() );
    frc::SmartDashboard::PutData("CmdDriveStr8", new CmdDriveStr8() );
}

frc::Joystick* OI::DriverGamepad() {
  return driver_gamepad;
}

frc::Joystick* OI::OperatorGamepad(){
  return operator_gamepad;
}