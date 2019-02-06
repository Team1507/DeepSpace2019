

#pragma once

#include "frc/commands/InstantCommand.h"

class CmdDriveClearAll : public frc::InstantCommand {
 public:
  CmdDriveClearAll();
  void Initialize() override;
};
