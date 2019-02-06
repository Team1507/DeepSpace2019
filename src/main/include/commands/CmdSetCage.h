
#pragma once

#include <frc/commands/Command.h>

class CmdSetCage : public frc::Command {
 public:
  CmdSetCage(int position);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;
  private:
  int m_position;
};
