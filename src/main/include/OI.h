#pragma once
#include <frc/WPILib.h>
class OI {
  private:
    frc::Joystick *driver_gamepad;
    frc::Joystick *operator_gamepad;
  public:
    OI();
    frc::Joystick* DriverGamepad();
    frc::Joystick* OperatorGamepad();
};