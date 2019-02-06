

#pragma once

#include <frc/commands/Command.h>
#include "Robot.h"

class CmdElevatorGo2Bot : public frc::Command {
 public:
  CmdElevatorGo2Bot();

  void Initialize() override;
  void Execute() override;
	bool IsFinished() override;
	void End() override;
	void Interrupted() override;

  private:
  
};
