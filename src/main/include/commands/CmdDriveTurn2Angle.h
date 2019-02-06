

#pragma once

#include <frc/Commands/Command.h>

class CmdDriveTurn2Angle : public frc::Command {
 public:
  CmdDriveTurn2Angle(double power, double angle);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;
private:
    double m_power;
    double m_angle;
    double m_calcAngle;
    bool   m_isRightTurn;
};
