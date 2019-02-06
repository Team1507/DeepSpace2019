

#pragma once

#include <frc/commands/Command.h>
#include "Robot.h"

class CmdElevatorGo2CargoMid : public frc::Command {
 public:
  CmdElevatorGo2CargoMid();

  void Initialize() override;
  void Execute() override;
	bool IsFinished() override;
	void End() override;
	void Interrupted() override;

  private:
  
};
