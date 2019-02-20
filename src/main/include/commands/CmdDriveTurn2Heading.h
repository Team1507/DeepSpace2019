

#pragma once

#include <frc/commands/Command.h>

class CmdDriveTurn2Heading : public frc::Command {
 public:
  CmdDriveTurn2Heading(double leftPower, double rightPower, double heading, double timeout); 
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

private:
    double m_leftPower;
    double m_rightPower;
    double m_heading;
    double m_timeout;

    bool   m_isRightTurn;

};
