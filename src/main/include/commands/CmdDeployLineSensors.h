

#pragma once

#include <frc/commands/InstantCommand.h>
#include "Robot.h"

class CmdDeployLineSensors : public frc::InstantCommand {
 public:
  CmdDeployLineSensors(bool value);
  void Initialize() override;
private:
  bool m_value;
};
