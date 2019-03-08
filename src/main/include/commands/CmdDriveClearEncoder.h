

#pragma once

#include "frc/commands/InstantCommand.h"

class CmdDriveClearEncoder : public frc::InstantCommand {
 public:
  CmdDriveClearEncoder();
  void Initialize() override;
};
