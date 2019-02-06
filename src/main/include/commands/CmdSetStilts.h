
#pragma once

#include <frc/commands/Command.h>

class CmdSetStilts : public frc::Command {
 public:
  CmdSetStilts(int position);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;
  private:
  int m_position;
};
