

#pragma once

#include <frc/commands/Command.h>

class CmdBuzzGamepad : public frc::Command {
 public:
    CmdBuzzGamepad();
    
    void Initialize() override;
    void Execute() override;
    bool IsFinished() override;
    void End() override;
    void Interrupted() override;


};
