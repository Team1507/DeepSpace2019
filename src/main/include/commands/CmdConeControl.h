#pragma once

#include <frc/commands/Command.h>

class CmdConeControl : public frc::Command {
  public:
    CmdConeControl(bool coneUp);
    void Initialize() override;
    void Execute() override;
    bool IsFinished() override;
    void End() override;
    void Interrupted() override;
  private:
    bool m_coneUp;
};
