

#pragma once

#include <frc/commands/Command.h>

class CmdTurn2Line : public frc::Command {
 public:
  CmdTurn2Line(bool isLeftTurn, double throttle);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;
  private:
  bool m_isLeftTurn;
  double m_throttle;
};
