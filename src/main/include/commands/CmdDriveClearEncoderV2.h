

#pragma once

#include <frc/commands/Command.h>

class CmdDriveClearEncoderV2 : public frc::Command {
 public:
    CmdDriveClearEncoderV2();
    
    void Initialize() override;
    void Execute() override;
    bool IsFinished() override;
    void End() override;
    void Interrupted() override;
  private:
    double m_left;
    double m_right;
};
