

#pragma once

#include <frc/commands/Command.h>

class CmdDriveWithGamepad : public frc::Command {
 public:
  CmdDriveWithGamepad();
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;
};
