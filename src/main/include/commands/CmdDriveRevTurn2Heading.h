
#pragma once

#include <frc/commands/Command.h>

class CmdDriveRevTurn2Heading : public frc::Command {
 public:
  CmdDriveRevTurn2Heading(double power, double heading);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

private:
    double m_power;
    double m_heading;

    bool   m_isRightTurn;

};
