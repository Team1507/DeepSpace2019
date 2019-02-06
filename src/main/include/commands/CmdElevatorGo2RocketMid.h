
#pragma once

#include <frc/commands/Command.h>
#include "Robot.h"

class CmdElevatorGo2RocketMid : public frc::Command {
 public:
  CmdElevatorGo2RocketMid();

  void Initialize() override;
  void Execute() override;
	bool IsFinished() override;
	void End() override;
	void Interrupted() override;

  private:
  
};
