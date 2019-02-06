#include "OI.h"
#include <frc/WPILib.h>
OI::OI() {
  driver_gamepad = new frc::Joystick(0);
  operator_gamepad = new frc::Joystick(1);
}

frc::Joystick* OI::DriverGamepad() {
  return driver_gamepad;
}

frc::Joystick* OI::OperatorGamepad(){
  return operator_gamepad;
}