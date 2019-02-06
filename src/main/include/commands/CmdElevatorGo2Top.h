

#pragma once

#include <frc/commands/Command.h>
#include "Robot.h"

class CmdElevatorGo2Top : public frc::Command {
 public:
  CmdElevatorGo2Top();

  void Initialize() override;
  void Execute() override;
	bool IsFinished() override;
	void End() override;
	void Interrupted() override;

  private:
  
};
