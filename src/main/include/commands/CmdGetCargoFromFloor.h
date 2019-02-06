
#pragma once

#include <frc/commands/CommandGroup.h>

class CmdGetCargoFromFloor : public frc::Command {
 public:
  CmdGetCargoFromFloor();
  void Initialize() override;
  void Execute() override;
	bool IsFinished() override;
	void End() override;
	void Interrupted() override;
  private:
  int m_state;
  int m_nextState; //This is used for exiting the timer properly
  int m_delayIterations;
};
